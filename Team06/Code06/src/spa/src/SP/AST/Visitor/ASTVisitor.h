#pragma once

#include <memory>

// general entry point of programs
class Program;
class Procedure;

// statements
class ReadStatement;
class PrintStatement;
class AssignStatement;
class CallStatement;
class IfElseStatement;
class WhileStatement;

// interface for visitor design pattern 
class ASTVisitor {
public:
	// general entry point of programs
	virtual void visitProgram(std::shared_ptr<Program> program) = 0;
	virtual void visitProcedure(std::shared_ptr<Procedure> procedure) = 0;

	// statements
	virtual void visitReadStatement(std::shared_ptr<ReadStatement> readStatement) = 0;
	virtual void visitPrintStatement(std::shared_ptr<PrintStatement> printStatement) = 0;
	virtual void visitAssignStatement(std::shared_ptr<AssignStatement> assignStatement) = 0;
	virtual void visitCallStatement(std::shared_ptr<CallStatement> callStatement) = 0;
	virtual void visitIfElseStatement(std::shared_ptr<IfElseStatement> IfElseStatement) = 0;
	virtual void visitWhileStatement(std::shared_ptr<WhileStatement> whileStatement) = 0;
};