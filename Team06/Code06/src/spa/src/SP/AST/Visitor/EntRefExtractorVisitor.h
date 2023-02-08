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

	void visitProgram(Program* program) override;
	void visitProcedure(Procedure* procedure) override;
	void visitStatementList(StatementList* statement) override;

	void visitStatement(Statement* statement) override;
	void visitReadStatement(ReadStatement* readStatement) override;
	void visitPrintStatement(PrintStatement* printStatement) override;
	void visitAssignStatement(AssignStatement* assignStatement) override;
	void visitCallStatement(CallStatement* callStatement) override;
	void visitIfStatement(IfStatement* ifStatement) override;
	void visitWhileStatement(WhileStatement* whileStatement) override;

	void visitExpression(Expression* expression) override;
	void visitConditionalExpression(ConditionalExpression* conditionalExpression) override;
	void visitCondExprHelper(ConditionalExpression* e, std::unordered_set<std::string>& variables, std::unordered_set<int>& constants);
	void visitExprHelper(Expression* e, std::unordered_set<std::string>& variables, std::unordered_set<int>& constants);
};