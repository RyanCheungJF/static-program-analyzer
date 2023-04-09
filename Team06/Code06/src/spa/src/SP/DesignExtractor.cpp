#include "DesignExtractor.h"

DesignExtractor::DesignExtractor() : ASTroot(), writePkb(), readPkb() {}

DesignExtractor::DesignExtractor(std::unique_ptr<Program> root, WritePKB* writePKB, ReadPKB* readPKB)
    : ASTroot(std::move(root)), writePkb(writePKB), readPkb(readPKB) {}

void DesignExtractor::populatePKB() {
    try {
        auto semanticValidator = SemanticValidator(ASTroot.get(), writePkb, readPkb);
        auto topoOrder = semanticValidator.validate();
        extractInfo(topoOrder);
        extractCFG();
    } catch (SemanticErrorException& e) {
        throw;
    }
}

void DesignExtractor::extractInfo(std::vector<ProcName> topoOrder) {
    auto followsExtractor = FollowsExtractorVisitor(writePkb);
    auto parentExtractor = ParentExtractorVisitor(writePkb);
    auto statementExtractor = StatementExtractorVisitor(writePkb);
    auto procedureExtractor = ProcedureExtractorVisitor(writePkb);
    auto entRefExtractor = EntRefExtractorVisitor(writePkb);
    std::vector<ASTVisitor*> visitors{&followsExtractor, &parentExtractor, &statementExtractor, &procedureExtractor,
                                      &entRefExtractor};

    for (const auto& visitor : visitors) {
        for (const auto& procedure : ASTroot->procedureList) {
            procedure->accept(visitor);
            procedure->statementList->accept(visitor);
            for (const auto& statement : procedure->getStatements()) {
                statement->accept(visitor);
                if (isContainerStatement(statement.get())) {
                    recurseStatementHelper(statement.get(), visitor);
                }
            }
        }
    }

    auto usesModifiesExtractor = UsesModifiesExtractor(writePkb, readPkb);
    usesModifiesExtractor.extract(topoOrder);
}

void DesignExtractor::extractCFG() {
    auto cfgBuilder = CFGBuilder(writePkb);
    for (const auto& procedure : ASTroot->procedureList) {
        cfgBuilder.buildCFG(procedure.get());
    }
}