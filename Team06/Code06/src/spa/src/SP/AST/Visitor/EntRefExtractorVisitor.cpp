#include "EntRefExtractorVisitor.h"

EntRefExtractorVisitor::EntRefExtractorVisitor(WritePKB* writePKB) {
	writeApi = writePKB;
}

void EntRefExtractorVisitor::visitProgram(const Program* program) {}

void EntRefExtractorVisitor::visitProcedure(const Procedure* procedure) {}

void EntRefExtractorVisitor::visitStatementList(const StatementList* statementList) {}

void EntRefExtractorVisitor::visitStatement(const Statement* statement) {}

void EntRefExtractorVisitor::visitReadStatement(const ReadStatement* readStatement) {

}

void EntRefExtractorVisitor::visitPrintStatement(const PrintStatement* printStatement) {

}

void EntRefExtractorVisitor::visitAssignStatement(const AssignStatement* assignStatement) {

}

void EntRefExtractorVisitor::visitCallStatement(const CallStatement* callStatement) {

}

void EntRefExtractorVisitor::visitIfStatement(const IfStatement* ifStatement) {
}

void EntRefExtractorVisitor::visitWhileStatement(const WhileStatement* whileStatement) {
}

void EntRefExtractorVisitor::visitExpression(const Expression* variable) {};

void EntRefExtractorVisitor::visitConditionalExpression(const ConditionalExpression* conditionalExpression) {};