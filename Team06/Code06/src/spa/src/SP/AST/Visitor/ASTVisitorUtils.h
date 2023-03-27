#pragma once

#include <unordered_set>

#include "PKB/ReadPKB.h"
#include "PKB/WritePKB.h"
#include "SP/AST/Node/AssignStatement.h"
#include "SP/AST/Node/BinaryConditionalExpression.h"
#include "SP/AST/Node/CallStatement.h"
#include "SP/AST/Node/Constant.h"
#include "SP/AST/Node/IfStatement.h"
#include "SP/AST/Node/MathExpression.h"
#include "SP/AST/Node/NotConditionalExpression.h"
#include "SP/AST/Node/PrintStatement.h"
#include "SP/AST/Node/Procedure.h"
#include "SP/AST/Node/Program.h"
#include "SP/AST/Node/ReadStatement.h"
#include "SP/AST/Node/RelationalExpression.h"
#include "SP/AST/Node/Statement.h"
#include "SP/AST/Node/StatementList.h"
#include "SP/AST/Node/Variable.h"
#include "SP/AST/Node/WhileStatement.h"

StmtNum visitLastStatementHelper(Statement* statement);
StmtNum checkLastStatementHelper(StatementList* stmtList);
void recurseStatementHelper(Statement* recurseStmt, ASTVisitor* visitor);
void checkStatementHelper(std::vector<std::unique_ptr<Statement>>& statements, ASTVisitor* visitor);
void visitCondExprHelper(ConditionalExpression* condExpr, std::unordered_set<Ent>& variables,
                         std::unordered_set<Const>& constants);
void visitExprHelper(Expression* expr, std::unordered_set<Ent>& variables, std::unordered_set<Const>& constants);
bool isContainerStatement(Statement* statement);
