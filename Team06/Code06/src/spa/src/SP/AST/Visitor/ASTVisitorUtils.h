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

int visitIfStatementHelper(IfStatement* ifStatement);
int visitWhileStatementHelper(WhileStatement* whileStatement);
void recurseStatementHelper(Statement* recurseStmt, ASTVisitor* visitor);
void visitCondExprHelper(ConditionalExpression* e, std::unordered_set<Ent>& variables,
                         std::unordered_set<int>& constants);
void visitExprHelper(Expression* e, std::unordered_set<Ent>& variables, std::unordered_set<int>& constants);
void populateRemainingTables(WritePKB* writePKB, ReadPKB* readPKB);
void populateUsesModifies(WritePKB* writePKB, ReadPKB* readPKB);
bool isContainerStatement(Statement* statement);
std::vector<std::unordered_set<Ent>> handleCallStmt(WritePKB* writePKB, ReadPKB* readPKB,
                                                    std::pair<StmtNum, ProcName> callStmt);
void buildCFG(Procedure* proc, WritePKB* writePKB, ReadPKB* readPKB);
void buildCFGHelper(std::unordered_map<StmtNum, std::pair<std::vector<StmtNum>, std::vector<StmtNum>>>& cfg,
                    StatementList* stmtList, StmtNum loopedStmtNum);
void validateNoDuplicateProcedureName(std::vector<ProcName> procedureNames);
void validateCalledProceduresExist(std::vector<ProcName> procedureNames,
                                   std::unordered_map<ProcName, std::vector<ProcName>> procCallMap);
void validateNoCycles(std::vector<ProcName> procedureNames,
                      std::unordered_map<ProcName, std::vector<ProcName>> procCallMap);
void recurseCallStatementHelper(Statement* recurseStmt,
                                std::unordered_map<ProcName, std::vector<ProcName>>& procCallMap,
                                ProcName parentProcedure);