#include "EntRefExtractorVisitor.h"

EntRefExtractorVisitor::EntRefExtractorVisitor(WritePKB* writePKB) {
	writeApi = writePKB;
}

void EntRefExtractorVisitor::visitProgram(const Program* program) {}

void EntRefExtractorVisitor::visitProcedure(const Procedure* procedure) {}

void EntRefExtractorVisitor::visitStatementList(const StatementList* statementList) {}

void EntRefExtractorVisitor::visitStatement(const Statement* statement) {}

void EntRefExtractorVisitor::visitReadStatement(const ReadStatement* readStatement) {
	std::vector<Ent> variables = { readStatement->varName };
	writeApi->setEntity(readStatement->statementNumber, variables);
}

void EntRefExtractorVisitor::visitPrintStatement(const PrintStatement* printStatement) {
	std::vector<Ent> variables = { printStatement->varName };
	writeApi->setEntity(printStatement->statementNumber, variables);
}

void EntRefExtractorVisitor::visitAssignStatement(const AssignStatement* assignStatement) {
	std::unordered_set<std::string> variables;
	std::unordered_set<int> constants;

	variables.insert(assignStatement->varName);
	visitExprHelper(assignStatement->expr.get(), variables, constants);

	std::vector<std::string> outputVariables(variables.begin(), variables.end());
	std::vector<int> outputConstants(constants.begin(), constants.end());
	writeApi->setEntity(assignStatement->statementNumber, outputVariables);
	writeApi->setConstant(assignStatement->statementNumber, outputConstants);
}

void EntRefExtractorVisitor::visitCallStatement(const CallStatement* callStatement) {}

void EntRefExtractorVisitor::visitIfStatement(const IfStatement* ifStatement) {
	std::unordered_set<std::string> variables;
	std::unordered_set<int> constants;

	visitCondExprHelper(ifStatement->condExpr.get(), variables, constants);

	std::vector<std::string> outputVariables(variables.begin(), variables.end());
	std::vector<int> outputConstants(constants.begin(), constants.end());

	writeApi->setEntity(ifStatement->statementNumber, outputVariables);
	writeApi->setConstant(ifStatement->statementNumber, outputConstants);
}

void EntRefExtractorVisitor::visitWhileStatement(const WhileStatement* whileStatement) {
	std::unordered_set<std::string> variables;
	std::unordered_set<int> constants;

	visitCondExprHelper(whileStatement->condExpr.get(), variables, constants);

	std::vector<std::string> outputVariables(variables.begin(), variables.end());
	std::vector<int> outputConstants(constants.begin(), constants.end());

	writeApi->setEntity(whileStatement->statementNumber, outputVariables);
	writeApi->setConstant(whileStatement->statementNumber, outputConstants);
}

void EntRefExtractorVisitor::visitExpression(const Expression* variable) {};

void EntRefExtractorVisitor::visitConditionalExpression(const ConditionalExpression* conditionalExpression) {};

void EntRefExtractorVisitor::visitCondExprHelper(ConditionalExpression* e, std::unordered_set<std::string>& variables, std::unordered_set<int>& constants) {
	if (auto i = dynamic_cast<NotConditionalExpression*>(e)) {
		visitCondExprHelper(i->condExpr.get(), variables, constants);
	}
	else if (auto i = dynamic_cast<BinaryConditionalExpression*>(e)) {
		visitCondExprHelper(i->lhs.get(), variables, constants);
		visitCondExprHelper(i->rhs.get(), variables, constants);
	}
	else if (auto i = dynamic_cast<RelationalExpression*>(e)) {
		visitExprHelper(i->lhs.get(), variables, constants);
		visitExprHelper(i->rhs.get(), variables, constants);
	}
}

void EntRefExtractorVisitor::visitExprHelper(Expression* e, std::unordered_set<std::string>& variables, std::unordered_set<int>& constants) {
	if (auto i = dynamic_cast<MathExpression*>(e)) {
		 visitExprHelper(i->lhs.get(), variables, constants);
		 visitExprHelper(i->rhs.get(), variables, constants);
	}
	else if (auto i = dynamic_cast<Constant*>(e)) {
		 constants.insert(i->value);
	} else if (auto i = dynamic_cast<Variable*>(e)) {
		 variables.insert(i->name);
	}
}