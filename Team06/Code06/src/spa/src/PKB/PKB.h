#pragma once

#include "../qps/entities/Pattern.h"
#include "../qps/entities/Relationship.h"
#include "../utils/AppConstants.h"
#include "readHandlers/AssignPatternHandler.h"
#include "readHandlers/FollowsParentHandler.h"
#include "readHandlers/ModifiesUsesHandler.h"
#include "readHandlers/CallsHandler.h"
#include "storage/CFGStorage.h"
#include "storage/CallStorage.h"
#include "storage/ConstantStorage.h"
#include "storage/EntityStorage.h"
#include "storage/FollowsParentStorage.h"
#include "storage/ModifiesUsesStorage.h"
#include "storage/PatternStorage.h"
#include "storage/ProcedureStorage.h"
#include "storage/StmtStorage.h"
#include "utils/AppConstants.h"
#include <iostream>
#include <queue>
#include <stdio.h>
#include <string>
#include <vector>

class PKB : AppConstants {
public:
  void initializePkb();

  // Sets Follows relation in PKB
  void setFollows(StmtNum left, StmtNum right);

  // Sets FollowsT relation in PKB
  void setFollowsT(StmtNum followee, std::unordered_set<StmtNum> followers);

  // Sets Parent relation in PKB
  void setParent(StmtNum parent, StmtNum child);

  // Sets ParentT relation in PKB
  void setParentT(StmtNum parent, std::unordered_set<StmtNum> children);

  // Sets the statement along with the statement line that it appears in
  void setStatement(Stmt s, StmtNum line);

  // Sets the entity along with the statement line that the entities appears in
  void setEntity(StmtNum line, std::unordered_set<Ent> entities);

  // Sets the entity along with the statement line that it appears in
  void setEntity(Ent e, StmtNum line);

  // Sets the procedure along with the statement lines that are in that
  // procedure appears in
  void setProcedure(ProcName p, std::unordered_set<StmtNum> lines);

  // Sets the constants along with the statement line that the constants appears
  // in
  void setConstant(StmtNum num, std::unordered_set<Const> constants);

  void setCallS(StmtNum callLine, ProcName procedure_being_called);

  void setCallP(ProcName caller, std::unordered_set<ProcName> callees);

  void setUsesS(StmtNum num, std::unordered_set<Ent> entities);

  void setUsesP(ProcName, std::unordered_set<Ent> entities);

  void setModifiesS(StmtNum num, std::unordered_set<Ent> entities);

  void setModifiesP(ProcName, std::unordered_set<Ent> entities);

  void writePattern(std::string lhs, StmtNum num, std::unique_ptr<Expression> pointer);

  void writeCFG(StmtNum num, CFGNodeStub &root);

  std::vector<std::vector<std::string>> findRelationship(shared_ptr<Relationship> rs);

  std::vector<std::string> findDesignEntities(Parameter p);

  // check if given a statement type and statement line number, whether that
  // statement line number is indeed of that statement type
  bool checkStatement(Stmt stmt, StmtNum num);

  // returns all the statement lines that are contained in the given procedure
  std::unordered_set<StmtNum> getProcedureStatementNumbers(ProcName p);

  // returns all the call statement lines and the procedure that it is calling
  std::vector<std::pair<StmtNum, ProcName>> getCallStatements();

  // returns all the procedure names present in the source code
  std::unordered_set<ProcName> getAllProcedureNames();

  // returns the entire row of all Entities involved in the Uses(StmtNum, v)
  // relationship
  std::unordered_set<Ent> getUsesS(StmtNum num);

  // returns the entire row of all Entities involved in the Modifies(StmtNum, v)
  // relationship
  std::unordered_set<Ent> getModifiesS(StmtNum num);

  // returns the entire row of all Entities involved in the Uses(ProcName, v)
  // relationship
  std::unordered_set<Ent> getUsesP(ProcName name);

  // returns the entire row of all Entities involved in the Modifies(ProcName,
  // v) relationship
  std::unordered_set<Ent> getModifiesP(ProcName name);

  // returns the name of the procedure being called on line number s
  // if line s is not a call statement, it returns a pair {-1, "INVALID"}
  std::pair<StmtNum, ProcName> getCallStmt(StmtNum s);

  // returns all statement numbers for if statement
  std::unordered_set<StmtNum> getIfStatementNumbers();

  // returns all statement numbers for while statement
  std::unordered_set<StmtNum> getWhileStatementNumbers();

  // returns nested statement numbers of all if and while statements
  std::unordered_set<StmtNum> getContainedStatements(StmtNum containerNum);

  // Returns relevant strings based on Pattern object passed
  std::vector<std::vector<std::string>> findPattern(Pattern p);

  // returns the root node of the CFG of the procedure if it exists. else it
  // returns a nullptr
  CFGNodeStub *getCFG(StmtNum num);

  // returns n2 where n2 satisfies Next(n1, n2)
  std::vector<StmtNum> getNextRHS(StmtNum n1);

  // returns n1 where n1 satisfies Next(n1, n2)
  std::vector<StmtNum> getNextLHS(StmtNum n2);

  // returns all n2 where n2 satisfies Next*(n1, n2)
  std::vector<StmtNum> getNextTRHS(StmtNum n1);

  // returns all n1 where n1 satisfies Next*(n1, n2)
  std::vector<StmtNum> getNextTLHS(StmtNum n2);

private:
  std::shared_ptr<FollowsParentStorage> followsStorage;
  std::shared_ptr<FollowsParentStorage> followsTStorage;
  std::shared_ptr<FollowsParentStorage> parentStorage;
  std::shared_ptr<FollowsParentStorage> parentTStorage;
  std::shared_ptr<StmtStorage> statementStorage;
  std::shared_ptr<EntityStorage> entityStorage;
  std::shared_ptr<ProcedureStorage> procedureStorage;
  std::shared_ptr<ConstantStorage> constantStorage;
  std::shared_ptr<PatternStorage> patternStorage;
  std::shared_ptr<CallStorage> callStorage;
  std::shared_ptr<ModifiesUsesStorage> usesStorage;
  std::shared_ptr<ModifiesUsesStorage> modifiesStorage;
  std::shared_ptr<CFGStorage> cfgStorage;

  std::unordered_map<RelationshipType, std::shared_ptr<FollowsParentStorage>> followsParentMap = {
      {RelationshipType::FOLLOWS, NULL},
      {RelationshipType::FOLLOWST, NULL},
      {RelationshipType::PARENT, NULL},
      {RelationshipType::PARENTT, NULL}};

  std::unordered_map<RelationshipType, std::shared_ptr<ModifiesUsesStorage>> modifiesUsesMap = {
      {RelationshipType::MODIFIES, NULL}, {RelationshipType::USES, NULL}};

  std::unordered_map<RelationshipType, std::shared_ptr<CallStorage>> callsMap = {
      {RelationshipType::CALLS, NULL},
      {RelationshipType::CALLST, NULL}};
};