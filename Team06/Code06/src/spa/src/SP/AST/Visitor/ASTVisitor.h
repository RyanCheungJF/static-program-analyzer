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

class Variable;
class Constant;
class RelationalExpression;
class ConditionalExpression;

// interface for visitor design pattern 
class ASTVisitor {
public:
	// general entry point of programs
	virtual void visitProgram(const Program *program) = 0;
	virtual void visitProcedure(const Procedure *procedure) = 0;
	virtual void visitStatementList(const StatementList *statementList) = 0;

	// statements
	virtual void visitStatement(const Statement *statement) = 0;
	virtual void visitReadStatement(const ReadStatement *readStatement) = 0;
	virtual void visitPrintStatement(const PrintStatement *printStatement) = 0;
	virtual void visitAssignStatement(const AssignStatement *assignStatement) = 0;
	virtual void visitCallStatement(const CallStatement *callStatement) = 0;
	virtual void visitIfStatement(const IfStatement *IfStatement) = 0;
	virtual void visitWhileStatement(const WhileStatement *whileStatement) = 0;

	virtual void visitVariable(const Variable* variable) = 0;
	virtual void visitConstant(const Constant* constant) = 0;
	virtual void visitRelationalExpression(const RelationalExpression* relExpression) = 0;
	virtual void visitConditionalExpression(const ConditionalExpression* conditionalExpression) = 0;
};