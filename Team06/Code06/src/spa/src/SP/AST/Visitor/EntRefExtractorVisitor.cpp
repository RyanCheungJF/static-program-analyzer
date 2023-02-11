#include "EntRefExtractorVisitor.h"

EntRefExtractorVisitor::EntRefExtractorVisitor(WritePKB* writePKB) {
	writeApi = writePKB;
}

void EntRefExtractorVisitor::visitProgram(Program* program) {}
void EntRefExtractorVisitor::visitProcedure(Procedure* procedure) {}
void EntRefExtractorVisitor::visitStatementList(StatementList* statementList) {}

void EntRefExtractorVisitor::visitReadStatement(ReadStatement* readStatement) {
	std::vector<Ent> variables = { readStatement->varName };
	writeApi->setEntity(readStatement->statementNumber, variables);
}

void EntRefExtractorVisitor::visitPrintStatement(PrintStatement* printStatement) {
	std::vector<Ent> variables = { printStatement->varName };
	writeApi->setEntity(printStatement->statementNumber, variables);
}

void EntRefExtractorVisitor::visitAssignStatement(AssignStatement* assignStatement) {
	std::unordered_set<Ent> variables;
	std::unordered_set<int> constants;

	variables.insert(assignStatement->varName);
	visitExprHelper(assignStatement->expr.get(), variables, constants);

	std::vector<Ent> outputVariables(variables.begin(), variables.end());
	std::vector<int> outputConstants(constants.begin(), constants.end());
	writeApi->setEntity(assignStatement->statementNumber, outputVariables);
	writeApi->setConstant(assignStatement->statementNumber, outputConstants);
	std::unique_ptr<Expression> expr;
	if (auto i = CAST_TO(MathExpression, assignStatement->expr.get())) {
		expr = std::make_unique<MathExpression>(std::move(*i));
	}
	else if (auto i = CAST_TO(Constant, assignStatement->expr.get())) {
		expr = std::make_unique<Constant>(std::move(*i));
	}
	else if (auto i = CAST_TO(Variable, assignStatement->expr.get())) {
		expr = std::make_unique<Variable>(std::move(*i));
	}
	writeApi->writePattern(assignStatement->varName, assignStatement->statementNumber, std::move(expr));
}

void EntRefExtractorVisitor::visitCallStatement(CallStatement* callStatement) {}

void EntRefExtractorVisitor::visitIfStatement(IfStatement* ifStatement) {
	std::unordered_set<Ent> variables;
	std::unordered_set<int> constants;

	visitCondExprHelper(ifStatement->condExpr.get(), variables, constants);

	std::vector<Ent> outputVariables(variables.begin(), variables.end());
	std::vector<int> outputConstants(constants.begin(), constants.end());

	writeApi->setEntity(ifStatement->statementNumber, outputVariables);
	writeApi->setConstant(ifStatement->statementNumber, outputConstants);
}

void EntRefExtractorVisitor::visitWhileStatement(WhileStatement* whileStatement) {
	std::unordered_set<Ent> variables;
	std::unordered_set<int> constants;

	visitCondExprHelper(whileStatement->condExpr.get(), variables, constants);

	std::vector<Ent> outputVariables(variables.begin(), variables.end());
	std::vector<int> outputConstants(constants.begin(), constants.end());

	writeApi->setEntity(whileStatement->statementNumber, outputVariables);
	writeApi->setConstant(whileStatement->statementNumber, outputConstants);
}

void EntRefExtractorVisitor::visitExpression(Expression* variable) {};
void EntRefExtractorVisitor::visitConditionalExpression(ConditionalExpression* conditionalExpression) {};

