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
void populateUsesModifies(WritePKB* writePKB, ReadPKB* readPKB);
void processCallStatements(WritePKB* writePKB, ReadPKB* readPKB);
void processContainerStatements(WritePKB* writePKB, ReadPKB* readPKB);
void processProcedures(WritePKB* writePKB, ReadPKB* readPKB);
bool isContainerStatement(Statement* statement);
std::vector<std::unordered_set<Ent>> handleCallStmt(WritePKB* writePKB, ReadPKB* readPKB,
                                                    std::pair<StmtNum, ProcName> callStmt);
void buildCFG(Procedure* proc, WritePKB* writePKB, ReadPKB* readPKB);
void buildCFGHelper(std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>& cfg,
                    StatementList* stmtList, StmtNum loopedStmtNum);
void connectNodesForCFG(std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>& cfg,
                        StmtNum curr, StmtNum next);
void validateNoDuplicateProcedureName(std::vector<ProcName>& procedureNames);
void validateCalledProceduresExist(std::vector<ProcName>& procedureNames,
                                   std::unordered_map<ProcName, std::unordered_set<ProcName>>& procCallMap);
void validateNoCycles(std::vector<ProcName>& procedureNames,
                      std::unordered_map<ProcName, std::unordered_set<ProcName>>& procCallMap, WritePKB* writePkb,
                      ReadPKB* readPkb);
void recurseCallStatementHelper(Statement* recurseStmt,
                                std::unordered_map<ProcName, std::unordered_set<ProcName>>& procCallMap,
                                ProcName parentProcedure);
void checkCallStatementHelper(std::vector<std::unique_ptr<Statement>>& statements,
                              std::unordered_map<ProcName, std::unordered_set<ProcName>>& procCallMap,
                              ProcName parentProcedure);
void populateCallsTable(std::unordered_map<ProcName, std::unordered_set<ProcName>>& procCallMap,
                        std::vector<ProcName>& order, WritePKB* writePKB, ReadPKB* readPKB);