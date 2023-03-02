#pragma once
#include "PKB.h"

class ReadPKB {
public:
  // Sets the pointer to the PKB instance if it is not set yet
  void setInstancePKB(PKB &pkb);

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

  // returns a pointer to the CFG graph
  std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> getCFG(ProcName name);

//  // returns n2 where n2 satisfies Next(n1, n2)
//  std::vector<StmtNum> getNextRHS(StmtNum n1);
//
//  // returns n1 where n1 satisfies Next(n1, n2)
//  std::vector<StmtNum> getNextLHS(StmtNum n2);
//
//  // returns all n2 where n2 satisfies Next*(n1, n2)
//  std::vector<StmtNum> getNextTRHS(StmtNum n1);
//
//  // returns all n1 where n1 satisfies Next*(n1, n2)
//  std::vector<StmtNum> getNextTLHS(StmtNum n2);

private:
  PKB *pkbInstance = NULL;
  std::unordered_map<RelationshipType, std::shared_ptr<FollowsParentStorage>> followsParentMap = {
      {RelationshipType::FOLLOWS, NULL},
      {RelationshipType::FOLLOWST, NULL},
      {RelationshipType::PARENT, NULL},
      {RelationshipType::PARENTT, NULL}};

  std::unordered_map<RelationshipType, std::shared_ptr<ModifiesUsesStorage>> modifiesUsesMap = {
      {RelationshipType::MODIFIES, NULL}, {RelationshipType::USES, NULL}};
};
