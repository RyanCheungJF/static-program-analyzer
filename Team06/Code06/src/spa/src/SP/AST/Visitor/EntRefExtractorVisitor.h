#pragma once

#include <iostream>
#include <vector>
#include "ASTVisitor.h"
#include "AST/Node/Program.h"
#include "AST/Node/Procedure.h"
#include "AST/Node/IfStatement.h"
#include "AST/Node/ReadStatement.h"
#include "AST/Node/CallStatement.h"
#include "AST/Node/AssignStatement.h"
#include "AST/Node/WhileStatement.h"
#include "AST/Node/PrintStatement.h"
#include "AST/Node/Statement.h"
#include "AST/Node/StatementList.h"
#include "AST/Node/WhileStatement.h"
#include "AST/Node/MathExpression.h"
#include "AST/Node/Constant.h"
#include "AST/Node/Variable.h"
#include "AST/Node/NotConditionalExpression.h"
#include "AST/Node/BinaryConditionalExpression.h"
#include "AST/Node/RelationalExpression.h"
#include "../../../PKB/WritePKB.h"

class EntRefExtractorVisitor : public ASTVisitor {
public:
	WritePKB* writeApi;

	EntRefExtractorVisitor(WritePKB* writePKB);

	void visitProgram(const Program* program) override;
	void visitProcedure(const Procedure* procedure) override;
	void visitStatementList(const StatementList* statement) override;

	void visitStatement(const Statement* statement) override;
	void visitReadStatement(const ReadStatement* readStatement) override;
	void visitPrintStatement(const PrintStatement* printStatement) override;
	void visitAssignStatement(const AssignStatement* assignStatement) override;
	void visitCallStatement(const CallStatement* callStatement) override;
	void visitIfStatement(const IfStatement* ifStatement) override;
	void visitWhileStatement(const WhileStatement* whileStatement) override;

	void visitExpression(const Expression* expression) override;
	void visitConditionalExpression(const ConditionalExpression* conditionalExpression) override;
	void visitCondExprHelper(ConditionalExpression* e, std::unordered_set<std::string>& variables, std::unordered_set<int>& constants);
	void visitExprHelper(Expression* e, std::unordered_set<std::string>& variables, std::unordered_set<int>& constants);
};