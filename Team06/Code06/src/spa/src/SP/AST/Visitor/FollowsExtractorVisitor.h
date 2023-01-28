#pragma once

#include <memory>
#include "ASTVisitor.h"

class FollowsExtractorVisitor : public ASTVisitor, public std::enable_shared_from_this<ASTVisitor> {
public:
	FollowsExtractorVisitor();

	void visitProgram(std::shared_ptr<Program> program);
	void visitProcedure(std::shared_ptr<Procedure> procedure);

	void visitReadStatement(std::shared_ptr<ReadStatement> readStatement);
	void visitPrintStatement(std::shared_ptr<PrintStatement> printStatement);
	void visitAssignStatement(std::shared_ptr<AssignStatement> assignStatement);
	void visitCallStatement(std::shared_ptr<CallStatement> callStatement);
	void visitIfElseStatement(std::shared_ptr<IfElseStatement> IfElseStatement);
	void visitWhileStatement(std::shared_ptr<WhileStatement> whileStatement);
};