#include "EntRefExtractorVisitor.h"

EntRefExtractorVisitor::EntRefExtractorVisitor(WritePKB* writePKB) : writeApi(writePKB) {}

void EntRefExtractorVisitor::visitProgram(Program* program) {}
void EntRefExtractorVisitor::visitProcedure(Procedure* procedure) {}
void EntRefExtractorVisitor::visitStatementList(StatementList* statementList) {}

void EntRefExtractorVisitor::visitReadStatement(ReadStatement* readStatement) {
    writeApi->setEntity(readStatement->statementNumber, {readStatement->varName});
    writeApi->setModifiesS(readStatement->statementNumber, {readStatement->varName});
}

void EntRefExtractorVisitor::visitPrintStatement(PrintStatement* printStatement) {
    writeApi->setEntity(printStatement->statementNumber, {printStatement->varName});
    writeApi->setUsesS(printStatement->statementNumber, {printStatement->varName});
}

void EntRefExtractorVisitor::visitAssignStatement(AssignStatement* assignStatement) {
    writeApi->setModifiesS(assignStatement->statementNumber, {assignStatement->varName});

    std::unordered_set<Ent> variables;
    std::unordered_set<Const> constants;
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

    containerStmtHelper(ifStatement->condExpr.get(), ifStatement->statementNumber, variables, constants);
    writeApi->setIfPattern(ifStatement->statementNumber, variables);
}

void EntRefExtractorVisitor::visitWhileStatement(WhileStatement* whileStatement) {
    std::unordered_set<Ent> variables;
    std::unordered_set<Const> constants;

    containerStmtHelper(whileStatement->condExpr.get(), whileStatement->statementNumber, variables, constants);
    writeApi->setWhilePattern(whileStatement->statementNumber, variables);
}

void EntRefExtractorVisitor::containerStmtHelper(ConditionalExpression* condExpr, StmtNum stmtNum,
                                                 std::unordered_set<Ent>& variables,
                                                 std::unordered_set<Const>& constants) {
    visitCondExprHelper(condExpr, variables, constants);
    writeApi->setEntity(stmtNum, variables);
    writeApi->setConstant(stmtNum, constants);
    writeApi->setUsesS(stmtNum, variables);
}

void EntRefExtractorVisitor::visitExpression(Expression* variable){};
void EntRefExtractorVisitor::visitConditionalExpression(ConditionalExpression* conditionalExpression){};