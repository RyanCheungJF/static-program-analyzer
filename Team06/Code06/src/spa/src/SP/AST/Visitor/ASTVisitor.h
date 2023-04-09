#pragma once

#define CAST_TO(ASTNodeType, value) (dynamic_cast<ASTNodeType*>(value))

// general entry point of programs
class Program;
class Procedure;
class StatementList;

// statements
class ReadStatement;
class PrintStatement;
class AssignStatement;
class CallStatement;
class IfStatement;
class WhileStatement;

class Expression;
class ConditionalExpression;

// interface for visitor design pattern
class ASTVisitor {
public:
    // general entry point of programs
    virtual void visitProgram(Program* program) = 0;
    virtual void visitProcedure(Procedure* procedure) = 0;
    virtual void visitStatementList(StatementList* statementList) = 0;

    // statements
    virtual void visitReadStatement(ReadStatement* readStatement) = 0;
    virtual void visitPrintStatement(PrintStatement* printStatement) = 0;
    virtual void visitAssignStatement(AssignStatement* assignStatement) = 0;
    virtual void visitCallStatement(CallStatement* callStatement) = 0;
    virtual void visitIfStatement(IfStatement* IfStatement) = 0;
    virtual void visitWhileStatement(WhileStatement* whileStatement) = 0;

    virtual void visitExpression(Expression* variable) = 0;
    virtual void visitConditionalExpression(ConditionalExpression* conditionalExpression) = 0;
};