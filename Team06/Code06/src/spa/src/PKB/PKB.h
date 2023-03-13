#pragma once

#include <stdio.h>

#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "../../unit_testing/src/stubs/With.h"
#include "../QPS/entities/Pattern.h"
#include "../QPS/entities/Relationship.h"
#include "../utils/AppConstants.h"
#include "readHandlers/AssignPatternHandler.h"
#include "readHandlers/CallsHandler.h"
#include "readHandlers/FollowsParentHandler.h"
#include "readHandlers/ModifiesUsesHandler.h"
#include "readHandlers/NextHandler.h"
#include "readHandlers/ifWhilePatternHandler.h"
#include "storage/CFGStorage.h"
#include "storage/CallStorage.h"
#include "storage/CallsStorage.h"
#include "storage/ConstantStorage.h"
#include "storage/EntityStorage.h"
#include "storage/FollowsParentStorage.h"
#include "storage/ModifiesUsesStorage.h"
#include "storage/PatternStorage.h"
#include "storage/PatternWithExprStorage.h"
#include "storage/ProcedureStorage.h"
#include "storage/StmtStorage.h"
#include "utils/AppConstants.h"
#include "cache/RelationshipCache.h"
#include "cache/ParameterCache.h"
#include "cache/PatternCache.h"

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

    void setCall(StmtNum callLine, ProcName procedure_being_called);

    void setCalls(ProcName caller, std::unordered_set<ProcName> callees);

    void setCallsT(ProcName caller, std::unordered_set<ProcName> callees);

    void setUsesS(StmtNum num, std::unordered_set<Ent> entities);

    void setUsesP(ProcName, std::unordered_set<Ent> entities);

    void setModifiesS(StmtNum num, std::unordered_set<Ent> entities);

    void setModifiesP(ProcName, std::unordered_set<Ent> entities);

    void writeIfPattern(StmtNum num, std::unordered_set<Ent> variables);

    void writeWhilePattern(StmtNum num, std::unordered_set<Ent> variables);

    void writePattern(std::string lhs, StmtNum num, std::unique_ptr<Expression> pointer);

    void writeCFG(ProcName name,
                  std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph);

    std::vector<std::vector<std::string>> findRelationship(shared_ptr<Relationship> rs);

    std::vector<std::string> findDesignEntities(Parameter p);

    // Returns relevant strings based on Pattern object passed
    std::vector<std::vector<std::string>> findPattern(Pattern p);

    std::vector<std::vector<std::string>> findAttribute(With w);

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

    // returns all the procedures that are called from a given procedure
    std::unordered_set<ProcName> getCallsT(ProcName p);

    // returns the cfg if it exists, else it returns an empty graph
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> getCFG(ProcName name);

private:
    // STATEMENTS
    std::shared_ptr<FollowsParentStorage> followsStorage;
    std::shared_ptr<FollowsParentStorage> followsTStorage;
    std::shared_ptr<FollowsParentStorage> parentStorage;
    std::shared_ptr<FollowsParentStorage> parentTStorage;
    std::shared_ptr<StmtStorage> statementStorage;
    std::shared_ptr<EntityStorage> entityStorage;
    std::shared_ptr<ProcedureStorage> procedureStorage;
    std::shared_ptr<ConstantStorage> constantStorage;
    std::shared_ptr<PatternWithExprStorage> assignPatternStorage;
    std::shared_ptr<PatternStorage> ifPatternStorage;
    std::shared_ptr<PatternStorage> whilePatternStorage;
    std::shared_ptr<CallStorage> callStorage;

    // RELATIONSHIPS
    std::shared_ptr<ModifiesUsesStorage> usesStorage;
    std::shared_ptr<ModifiesUsesStorage> modifiesStorage;
    std::shared_ptr<CallsStorage> callsStorage;
    std::shared_ptr<CallsStorage> callsTStorage;
    std::shared_ptr<CFGStorage> cfgStorage;

    RelationshipCache relationshipCache;
    ParameterCache parameterCache;
    PatternCache patternCache;


    std::unordered_map<RelationshipType, std::shared_ptr<FollowsParentStorage>> followsParentMap = {
        {RelationshipType::FOLLOWS, NULL},
        {RelationshipType::FOLLOWST, NULL},
        {RelationshipType::PARENT, NULL},
        {RelationshipType::PARENTT, NULL}};

    std::unordered_map<RelationshipType, std::shared_ptr<ModifiesUsesStorage>> modifiesUsesMap = {
        {RelationshipType::MODIFIES, NULL}, {RelationshipType::USES, NULL}};

    std::unordered_map<ParameterType, std::shared_ptr<PatternStorage>> ifWhilePatternMap = {
        {ParameterType::IF, NULL}, {ParameterType::WHILE, NULL}};

    std::unordered_map<RelationshipType, std::shared_ptr<CallsStorage>> callsMap = {{RelationshipType::CALLS, NULL},
                                                                                    {RelationshipType::CALLST, NULL}};

    std::unordered_map<RelationshipType, std::shared_ptr<CallsStorage>> nextMap = {{RelationshipType::NEXT, NULL},
                                                                                   {RelationshipType::NEXTT, NULL}};
};