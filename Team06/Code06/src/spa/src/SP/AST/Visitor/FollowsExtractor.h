#pragma once

#include "ASTVisitor.h"

class FollowsExtractor : public ASTVisitor {
public:
	FollowsExtractor();

	void visitProgram(Program& program);
	void visitProcedure(Procedure& procedure);

	void visitReadStatement(ReadStatement& readStatement);
	void visitPrintStatement(PrintStatement& printStatement);
	void visitAssignStatement(AssignStatement& assignStatement);
	void visitCallStatement(CallStatement& callStatement);
	void visitIfStatement(IfStatement& IfStatement);
	void visitWhileStatement(WhileStatement& whileStatement);
};