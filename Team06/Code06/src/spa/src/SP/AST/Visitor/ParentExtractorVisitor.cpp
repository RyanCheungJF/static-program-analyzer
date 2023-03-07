#include "ParentExtractorVisitor.h"

ParentExtractorVisitor::ParentExtractorVisitor(WritePKB* writePKB) {
    writeApi = writePKB;
}

void ParentExtractorVisitor::visitProgram(Program* program) {}
void ParentExtractorVisitor::visitProcedure(Procedure* procedure) {}
void ParentExtractorVisitor::visitStatementList(StatementList* statementList) {}
void ParentExtractorVisitor::visitReadStatement(ReadStatement* readStatement) {}
void ParentExtractorVisitor::visitPrintStatement(PrintStatement* printStatement) {}
void ParentExtractorVisitor::visitAssignStatement(AssignStatement* assignStatement) {}
void ParentExtractorVisitor::visitCallStatement(CallStatement* callStatement) {}

void ParentExtractorVisitor::visitIfStatement(IfStatement* ifStatement) {
    // For Parent
    for (auto const& child : ifStatement->getThenStatements()) {
        writeApi->setParent(ifStatement->statementNumber, child->statementNumber);
    }
    for (auto const& child : ifStatement->getElseStatements()) {
        writeApi->setParent(ifStatement->statementNumber, child->statementNumber);
    }
    // For ParentT
    std::unordered_set<StmtNum> parentTSet;
    StmtNum start = ifStatement->statementNumber + 1;
    StmtNum end = visitLastStatementHelper(ifStatement);
    for (StmtNum i = start; i <= end; i++) {
        parentTSet.insert(i);
    }

    writeApi->setParentT(ifStatement->statementNumber, parentTSet);
}

void ParentExtractorVisitor::visitWhileStatement(WhileStatement* whileStatement) {
    // For Parent
    for (auto const& child : whileStatement->getStatements()) {
        writeApi->setParent(whileStatement->statementNumber, child->statementNumber);
    }
    // For ParentT
    std::unordered_set<StmtNum> parentTSet;
    StmtNum start = whileStatement->statementNumber + 1;
    StmtNum end = visitLastStatementHelper(whileStatement);
    for (StmtNum i = start; i <= end; i++) {
        parentTSet.insert(i);
    }

    writeApi->setParentT(whileStatement->statementNumber, parentTSet);
}

void ParentExtractorVisitor::visitExpression(Expression* variable) {}
void ParentExtractorVisitor::visitConditionalExpression(ConditionalExpression* conditionalExpression) {}