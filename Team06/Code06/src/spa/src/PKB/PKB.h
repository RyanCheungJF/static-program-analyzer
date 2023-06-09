#pragma once

#include <queue>

#include "../QPS/entities/Comparison.h"
#include "../QPS/entities/Pattern.h"
#include "../QPS/entities/Relationship.h"
#include "../utils/AppConstants.h"
#include "cache/AttributeCache.h"
#include "cache/ComparisonCache.h"
#include "cache/ParameterCache.h"
#include "cache/PatternCache.h"
#include "cache/RelationshipCache.h"
#include "readHandlers/AffectsHandler.h"
#include "readHandlers/AssignPatternHandler.h"
#include "readHandlers/CallsHandler.h"
#include "readHandlers/FollowsParentHandler.h"
#include "readHandlers/IfWhilePatternHandler.h"
#include "readHandlers/ModifiesUsesHandler.h"
#include "readHandlers/NextHandler.h"
#include "storage/CFGStorage.h"
#include "storage/CallStorage.h"
#include "storage/EntityStorage.h"
#include "storage/ModifiesUsesStorage.h"
#include "storage/PatternStorage.h"
#include "storage/PatternWithExprStorage.h"
#include "storage/ProcedureStorage.h"
#include "storage/RelationshipStorage.h"
#include "storage/StmtStorage.h"
#include "utils/AppConstants.h"

class PKB : AppConstants {
public:
    void initializePkb();

    // Sets Follows relation in PKB
    void setFollows(StmtNum left, StmtNum right);

    // Sets FollowsT relation in PKB
    void setFollowsT(StmtNum followee, std::unordered_set<StmtNum>& followers);

    // Sets Parent relation in PKB
    void setParent(StmtNum parent, StmtNum child);

    // Sets ParentT relation in PKB
    void setParentT(StmtNum parent, std::unordered_set<StmtNum>& children);

    // Sets the statement along with the statement line that it appears in
    void setStatement(Stmt s, StmtNum line);

    // Sets the entity along with the statement line that the entities appears in
    void setEntity(StmtNum line, std::unordered_set<Ent>& entities);

    // Sets the procedure along with the statement lines that are in that procedure it appears in
    void setProcedure(ProcName p, std::unordered_set<StmtNum>& lines);

    // Sets the procedure along with the assign statement lines that are in that procedure
    void setProcAssignStmt(ProcName p, StmtNum num);

    // Sets the constants along with the statement line that the constants appears in
    void setConstant(StmtNum num, std::unordered_set<Const>& constants);

    void setCall(StmtNum callLine, ProcName procedure_being_called);

    void setCalls(ProcName caller, std::unordered_set<ProcName>& callees);

    void setCallsT(ProcName caller, std::unordered_set<ProcName>& callees);

    void setUsesS(StmtNum num, std::unordered_set<Ent>& entities);

    void setUsesP(ProcName, std::unordered_set<Ent>& entities);

    void setModifiesS(StmtNum num, std::unordered_set<Ent>& entities);

    void setModifiesP(ProcName, std::unordered_set<Ent>& entities);

    void writeIfPattern(StmtNum num, std::unordered_set<Ent>& variables);

    void writeWhilePattern(StmtNum num, std::unordered_set<Ent>& variables);

    void writePattern(std::string lhs, StmtNum num, std::unique_ptr<Expression> pointer);

    void writeCFG(ProcName name,
                  std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>& graph);

    // Returns relevant results based on the type of Relationship object passed and its parameters.
    std::vector<std::vector<std::string>> findRelationship(std::shared_ptr<Relationship>& rs);

    // Returns relevant results based on the type of Parameter object passed.
    std::vector<std::string> findDesignEntities(Parameter& p);

    // Returns relevant results based on Pattern object passed.
    std::vector<std::vector<std::string>> findPattern(Pattern& p);

    // Returns relevant results based on the Attributes of the Parameter object passed.
    std::vector<std::vector<std::string>> findAttribute(Parameter& p);

    // Returns relevant results based on the Comparison object passed.
    std::vector<std::vector<std::string>> findWith(Comparison& c);

    // check if given a statement type and statement line number, whether that
    // statement line number is indeed of that statement type
    bool checkStatement(Stmt stmt, StmtNum num);

    // returns all the statement lines that are contained in the given procedure
    std::unordered_set<StmtNum>& getProcedureStatementNumbers(ProcName p);

    // returns all the procedure names present in the source code
    std::unordered_set<ProcName>& getAllProcedureNames();

    // returns the entire row of all Entities involved in the Uses(StmtNum, v) relationship
    std::unordered_set<Ent>& getUsesS(StmtNum num);

    // returns the entire row of all Entities involved in the Modifies(StmtNum, v) relationship
    std::unordered_set<Ent>& getModifiesS(StmtNum num);

    // returns the entire row of all Entities involved in the Uses(ProcName, v) relationship
    std::unordered_set<Ent>& getUsesP(ProcName name);

    // returns the entire row of all Entities involved in the Modifies(ProcName, v) relationship
    std::unordered_set<Ent>& getModifiesP(ProcName name);

    // returns the name of the procedure being called on line number s. if line s is not a call statement,
    // it returns a pair {AppConstants::NOT_USED_FIELD, AppConstants::PROCEDURE_DOES_NOT_EXIST}
    std::pair<StmtNum, ProcName> getCallStmt(StmtNum s);

    // returns all statement numbers for if statement
    std::unordered_set<StmtNum>& getIfStatementNumbers();

    // returns all statement numbers for while statement
    std::unordered_set<StmtNum>& getWhileStatementNumbers();

    // returns nested statement numbers of all if and while statements
    std::unordered_set<StmtNum>& getContainedStatements(StmtNum containerNum);

    // returns all the procedures that are called from a given procedure
    std::unordered_set<ProcName>& getCallsT(ProcName p);

    // returns the cfg if it exists, else it returns an empty graph
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>& getCFG(ProcName name);

    // clears the caches in PKB
    void clearCache();

private:
    // STATEMENTS
    std::shared_ptr<StmtStorage> statementStorage;
    std::shared_ptr<ProcedureStorage> procedureStorage;
    std::shared_ptr<ProcedureStorage> procAssignStmtStorage;
    std::shared_ptr<EntityStorage<Ent>> entityStorage;
    std::shared_ptr<EntityStorage<Const>> constantStorage;
    std::shared_ptr<CallStorage> callStorage;

    // RELATIONSHIPS
    std::shared_ptr<RelationshipStorage<StmtNum, StmtNum>> followsStorage;
    std::shared_ptr<RelationshipStorage<StmtNum, StmtNum>> followsTStorage;
    std::shared_ptr<RelationshipStorage<StmtNum, StmtNum>> parentStorage;
    std::shared_ptr<RelationshipStorage<StmtNum, StmtNum>> parentTStorage;
    std::shared_ptr<ModifiesUsesStorage> usesStorage;
    std::shared_ptr<ModifiesUsesStorage> modifiesStorage;
    std::shared_ptr<RelationshipStorage<Ent, Ent>> callsStorage;
    std::shared_ptr<RelationshipStorage<Ent, Ent>> callsTStorage;
    std::shared_ptr<CFGStorage> cfgStorage;

    // PATTERNS
    std::shared_ptr<PatternWithExprStorage> assignPatternStorage;
    std::shared_ptr<PatternStorage> ifPatternStorage;
    std::shared_ptr<PatternStorage> whilePatternStorage;

    // CACHES
    std::shared_ptr<RelationshipCache> relationshipCache;
    std::shared_ptr<ParameterCache> parameterCache;
    std::shared_ptr<PatternCache> patternCache;
    std::shared_ptr<AttributeCache> attributeCache;
    std::shared_ptr<ComparisonCache> comparisonCache;

    // HANDLERS
    std::shared_ptr<FollowsParentHandler> followsParentHandler;
    std::shared_ptr<ModifiesUsesHandler> modifiesUsesHandler;
    std::shared_ptr<CallsHandler> callsHandler;
    std::shared_ptr<AffectsHandler> affectsHandler;
    std::shared_ptr<NextHandler> nextHandler;
    std::shared_ptr<AssignPatternHandler> assignPatternHandler;
    std::shared_ptr<IfWhilePatternHandler> ifWhilePatternHandler;

    // MAPS
    std::unordered_map<RelationshipType, std::shared_ptr<RelationshipStorage<StmtNum, StmtNum>>> followsParentMap = {
        {RelationshipType::FOLLOWS, nullptr},
        {RelationshipType::FOLLOWST, nullptr},
        {RelationshipType::PARENT, nullptr},
        {RelationshipType::PARENTT, nullptr}};

    std::unordered_map<RelationshipType, std::shared_ptr<ModifiesUsesStorage>> modifiesUsesMap = {
        {RelationshipType::MODIFIES, nullptr}, {RelationshipType::USES, nullptr}};

    std::unordered_map<ParameterType, std::shared_ptr<PatternStorage>> ifWhilePatternMap = {
        {ParameterType::IF, nullptr}, {ParameterType::WHILE, nullptr}};

    std::unordered_map<RelationshipType, std::shared_ptr<RelationshipStorage<Ent, Ent>>> callsMap = {
        {RelationshipType::CALLS, nullptr}, {RelationshipType::CALLST, nullptr}};

    std::unordered_set<RelationshipType> nextMap = {RelationshipType::NEXT, RelationshipType::NEXTT};
    std::unordered_set<RelationshipType> affectsMap = {RelationshipType::AFFECTS, RelationshipType::AFFECTST};

    // HELPER FUNCTIONS
    void filterAttributeResult(std::vector<std::vector<std::string>>& res, std::string val);
};