#pragma once

#include "ASTVisitor.h"
#include "AST/Node/Program.h"
#include "AST/Node/Procedure.h"
#include "AST/Node/IfStatement.h"
#include "AST/Node/Statement.h"
#include "AST/Node/StatementList.h"
#include "AST/Node/WhileStatement.h"
#include "../../../PKB/WritePKB.h"

class FollowsExtractor : public ASTVisitor {
public:
	WritePKB* writeApi;

	FollowsExtractor(WritePKB* writePKB);

	void visitProgram(Program *program) override;
	void visitProcedure(Procedure* procedure) override;
	void visitStatementList(StatementList *statement) override;

	void visitReadStatement(ReadStatement *readStatement) override;
	void visitPrintStatement(PrintStatement *printStatement) override;
	void visitAssignStatement(AssignStatement *assignStatement) override;
	void visitCallStatement(CallStatement *callStatement) override;
	void visitIfStatement(IfStatement *ifStatement) override;
	void visitWhileStatement(WhileStatement *whileStatement) override;

	void visitExpression(Expression *expression) override;
	void visitConditionalExpression(ConditionalExpression *conditionalExpression) override;
};