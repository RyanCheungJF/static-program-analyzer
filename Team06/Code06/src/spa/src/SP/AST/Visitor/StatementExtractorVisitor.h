#pragma once

#include <iostream>
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

class StatementExtractorVisitor : public ASTVisitor {
public:
	void visitProgram(const Program* program) override;
	void visitProcedure(const Procedure* procedure) override;
	void visitStatementList(const StatementList* statement) override;

	void visitStatement(const Statement* statement) override;
	void visitReadStatement(const ReadStatement *readStatement) override;
	void visitPrintStatement(const PrintStatement* printStatement) override;
	void visitAssignStatement(const AssignStatement* assignStatement) override;
	void visitCallStatement(const CallStatement* callStatement) override;
	void visitIfStatement(const IfStatement* IfStatement) override;
	void visitWhileStatement(const WhileStatement* whileStatement) override;

	void visitExpression(const Expression* expression) override;
	void visitConditionalExpression(const ConditionalExpression* conditionalExpression) override;
};