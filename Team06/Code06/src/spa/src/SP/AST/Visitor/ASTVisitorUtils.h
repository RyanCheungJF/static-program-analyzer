#pragma once

#include <unordered_set>
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
#include "AST/Node/IfStatement.h"
#include "AST/Node/WhileStatement.h"

int visitIfStatementHelper(IfStatement* ifStatement);
int visitWhileStatementHelper(WhileStatement* whileStatement);
void recurseStatementHelper(Statement* recurseStmt, ASTVisitor* visitor);
void visitCondExprHelper(ConditionalExpression* e, std::unordered_set<std::string>& variables, std::unordered_set<int>& constants);
void visitExprHelper(Expression* e, std::unordered_set<std::string>& variables, std::unordered_set<int>& constants);