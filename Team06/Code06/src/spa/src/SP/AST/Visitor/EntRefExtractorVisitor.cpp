#include "EntRefExtractorVisitor.h"

EntRefExtractorVisitor::EntRefExtractorVisitor(WritePKB* writePKB) : writeApi(writePKB) {}

void EntRefExtractorVisitor::visitProgram(Program* program) {}
void EntRefExtractorVisitor::visitProcedure(Procedure* procedure) {}
void EntRefExtractorVisitor::visitStatementList(StatementList* statementList) {}

void EntRefExtractorVisitor::visitReadStatement(ReadStatement* readStatement) {
    std::unordered_set<Ent> variables = {readStatement->varName};
    writeApi->setEntity(readStatement->statementNumber, variables);
    writeApi->setModifiesS(readStatement->statementNumber, variables);
}

void EntRefExtractorVisitor::visitPrintStatement(PrintStatement* printStatement) {
    std::unordered_set<Ent> variables = {printStatement->varName};
    writeApi->setEntity(printStatement->statementNumber, variables);
    writeApi->setUsesS(printStatement->statementNumber, variables);
}

void EntRefExtractorVisitor::visitAssignStatement(AssignStatement* assignStatement) {
    std::unordered_set<Ent> variables;
    std::unordered_set<Const> constants;

    std::unordered_set<Ent> lhsVariables = {assignStatement->varName};
    writeApi->setModifiesS(assignStatement->statementNumber, lhsVariables);

    visitExprHelper(assignStatement->expr.get(), variables, constants);

    writeApi->setUsesS(assignStatement->statementNumber, variables);

    variables.insert(assignStatement->varName);
    writeApi->setEntity(assignStatement->statementNumber, variables);
    writeApi->setConstant(assignStatement->statementNumber, constants);

    // Write expression tree to pattern storage
    writeApi->writePattern(assignStatement->varName, assignStatement->statementNumber,
                           std::move(assignStatement->expr));
}

void EntRefExtractorVisitor::visitCallStatement(CallStatement* callStatement) {
    writeApi->setCall(callStatement->statementNumber, callStatement->procName);
}

void EntRefExtractorVisitor::visitIfStatement(IfStatement* ifStatement) {
    std::unordered_set<Ent> variables;
    std::unordered_set<Const> constants;

    visitCondExprHelper(ifStatement->condExpr.get(), variables, constants);
    writeApi->setIfPattern(ifStatement->statementNumber, variables);
    writeContainerStmtEntities(ifStatement->statementNumber, variables, constants);
}

void EntRefExtractorVisitor::visitWhileStatement(WhileStatement* whileStatement) {
    std::unordered_set<Ent> variables;
    std::unordered_set<Const> constants;

    visitCondExprHelper(whileStatement->condExpr.get(), variables, constants);
    writeApi->setWhilePattern(whileStatement->statementNumber, variables);
    writeContainerStmtEntities(whileStatement->statementNumber, variables, constants);
}

void EntRefExtractorVisitor::visitExpression(Expression* variable){};
void EntRefExtractorVisitor::visitConditionalExpression(ConditionalExpression* conditionalExpression){};

void EntRefExtractorVisitor::writeContainerStmtEntities(StmtNum stmtNum, std::unordered_set<Ent>& variables,
                                                        std::unordered_set<Const>& constants) {
    writeApi->setEntity(stmtNum, variables);
    writeApi->setConstant(stmtNum, constants);
    writeApi->setUsesS(stmtNum, variables);
}