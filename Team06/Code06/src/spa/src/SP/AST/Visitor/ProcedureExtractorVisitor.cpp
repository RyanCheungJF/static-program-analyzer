#include "ProcedureExtractorVisitor.h"

ProcedureExtractorVisitor::ProcedureExtractorVisitor(WritePKB* writePKB) : writeApi(writePKB) {}

void ProcedureExtractorVisitor::visitProgram(Program* program) {}

void ProcedureExtractorVisitor::visitProcedure(Procedure* procedure) {
    StmtNum firstStmtNum = procedure->getFirstStatementNumber();
    StmtNum lastStmtNum = procedure->getLastStatementNumber();

    // If last statement is a container statement, we recurse down further
    if (isContainerStatement(procedure->getLastStatement())) {
        lastStmtNum = visitLastStatementHelper(procedure->getLastStatement());
    }

    std::unordered_set<StmtNum> procStmtNumbers;
    for (StmtNum i = firstStmtNum; i <= lastStmtNum; i++) {
        procStmtNumbers.insert(i);
    }

    writeApi->setProcedure(procedure->procedureName, procStmtNumbers);
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