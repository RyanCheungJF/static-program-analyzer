#include "ProcedureExtractorVisitor.h"

ProcedureExtractorVisitor::ProcedureExtractorVisitor(WritePKB* writePKB) {
    writeApi = writePKB;
}

void ProcedureExtractorVisitor::visitProgram(Program* program) {}

void ProcedureExtractorVisitor::visitProcedure(Procedure* procedure) {
    StmtNum firstStatementNumber = procedure->getFirstStatementNumber();
    StmtNum lastStatementNumber = procedure->getLastStatementNumber();

    // if its an if or while statement, we recurse down further
    if (CAST_TO(IfStatement, procedure->getLastStatement())) {
        lastStatementNumber = visitIfStatementHelper(CAST_TO(IfStatement, procedure->getLastStatement()));
    }
    if (CAST_TO(WhileStatement, procedure->getLastStatement())) {
        lastStatementNumber = visitWhileStatementHelper(CAST_TO(WhileStatement, procedure->getLastStatement()));
    }

    std::unordered_set<StmtNum> statementNumbers;
    for (StmtNum i = firstStatementNumber; i <= lastStatementNumber; i++) {
        statementNumbers.insert(i);
    }

    writeApi->setProcedure(procedure->procedureName, statementNumbers);
}
void ProcedureExtractorVisitor::visitStatementList(StatementList* statementList) {}
void ProcedureExtractorVisitor::visitReadStatement(ReadStatement* readStatement) {}
void ProcedureExtractorVisitor::visitPrintStatement(PrintStatement* printStatement) {}
void ProcedureExtractorVisitor::visitAssignStatement(AssignStatement* assignStatement) {}
void ProcedureExtractorVisitor::visitCallStatement(CallStatement* callStatement) {}
void ProcedureExtractorVisitor::visitIfStatement(IfStatement* ifStatement) {}
void ProcedureExtractorVisitor::visitWhileStatement(WhileStatement* whileStatement) {}
void ProcedureExtractorVisitor::visitExpression(Expression* variable) {}
void ProcedureExtractorVisitor::visitConditionalExpression(ConditionalExpression* conditionalExpression) {}