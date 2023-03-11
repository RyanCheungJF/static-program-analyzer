#include "ParentExtractorVisitor.h"

ParentExtractorVisitor::ParentExtractorVisitor(WritePKB* writePKB) : writeApi(writePKB) {}

void ParentExtractorVisitor::visitProgram(Program* program) {}
void ParentExtractorVisitor::visitProcedure(Procedure* procedure) {}
void ParentExtractorVisitor::visitStatementList(StatementList* statementList) {}
void ParentExtractorVisitor::visitReadStatement(ReadStatement* readStatement) {}
void ParentExtractorVisitor::visitPrintStatement(PrintStatement* printStatement) {}
void ParentExtractorVisitor::visitAssignStatement(AssignStatement* assignStatement) {}
void ParentExtractorVisitor::visitCallStatement(CallStatement* callStatement) {}

void ParentExtractorVisitor::visitIfStatement(IfStatement* ifStatement) {
    // For Parent
    for (const auto& childStmt : ifStatement->getThenStatements()) {
        writeApi->setParent(ifStatement->statementNumber, childStmt->statementNumber);
    }
    for (const auto& childStmt : ifStatement->getElseStatements()) {
        writeApi->setParent(ifStatement->statementNumber, childStmt->statementNumber);
    }
    // For ParentT
    parentTHelper(ifStatement);
}

void ParentExtractorVisitor::visitWhileStatement(WhileStatement* whileStatement) {
    // For Parent
    for (const auto& childStmt : whileStatement->getStatements()) {
        writeApi->setParent(whileStatement->statementNumber, childStmt->statementNumber);
    }
    // For ParentT
    parentTHelper(whileStatement);
}

void ParentExtractorVisitor::parentTHelper(Statement* statement) {
    std::unordered_set<StmtNum> parentTSet;
    StmtNum firstStmtNum = statement->statementNumber + 1;
    StmtNum lastStmtNum = visitLastStatementHelper(statement);
    for (StmtNum i = firstStmtNum; i <= lastStmtNum; i++) {
        parentTSet.insert(i);
    }

    writeApi->setParentT(statement->statementNumber, parentTSet);
}

void ParentExtractorVisitor::visitExpression(Expression* variable) {}
void ParentExtractorVisitor::visitConditionalExpression(ConditionalExpression* conditionalExpression) {}
