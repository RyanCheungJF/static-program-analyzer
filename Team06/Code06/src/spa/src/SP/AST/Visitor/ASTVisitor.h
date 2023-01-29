#pragma once

// general entry point of programs
class Program;
class Procedure;
class StatementList;
class Statement;

// statements
class ReadStatement;
class PrintStatement;
class AssignStatement;
class CallStatement;
class IfStatement;
class WhileStatement;

// interface for visitor design pattern 
class ASTVisitor {
public:
	// general entry point of programs
	virtual void visitProgram(const Program *program) = 0;
	virtual void visitProcedure(const Procedure *procedure) = 0;
	virtual void visitStatementList(const StatementList *statementList) = 0;

	// statements
	virtual void visitStatement(const Statement *statement) = 0;
	virtual void visitReadStatement(ReadStatement& readStatement) = 0;
	virtual void visitPrintStatement(PrintStatement& printStatement) = 0;
	virtual void visitAssignStatement(AssignStatement& assignStatement) = 0;
	virtual void visitCallStatement(CallStatement& callStatement) = 0;
	virtual void visitIfStatement(IfStatement& IfStatement) = 0;
	virtual void visitWhileStatement(WhileStatement& whileStatement) = 0;
};