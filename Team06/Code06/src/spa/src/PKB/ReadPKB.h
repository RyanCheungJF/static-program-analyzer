#pragma once
#include "PKB.h"
#include "queryHandlers/StmtStmtRLHandler.h"
#include "queryHandlers/StmtEntRLHandler.h"
#include "queryHandlers/EntEntRLHandler.h"
#include "../qps/entities/Relationship.h"
#include "../qps/entities/Pattern.h"

//#ifndef SPA_READPKB_H
//#define SPA_READPKB_H
//#endif //SPA_READPKB_H

typedef int StmtNum;

class ReadPKB {
public:

    // Sets the pointer to the PKB instance if it is not set yet
    void setInstancePKB(PKB& pkb);

    // Returns relavant statement number strings based on the Relationship object passed
    std::vector<std::vector<std::string>> findRelationship(Relationship rs);

    // Returns a vector of strings based on the Parameter object passed
    std::vector<std::string> findDesignEntities(Parameter p);

    // Returns relevant strings based on Pattern object passed
    std::vector<std::vector<std::string>> findPattern(Pattern p);


    // check if given a statement type and statement line number, whether that statement line number is indeed
    // of that statement type
    bool checkStatement(Stmt stmt, StmtNum num);

    // returns all the statement lines of a statement
    std::unordered_set<StmtNum> getStatementNumbers(Stmt stmt);


    // check if given an entity is indeed in the given statement line number
    bool checkEntity(Ent e, StmtNum num);

    // returns all the statement lines that an entity appears in
    std::unordered_set<StmtNum> getEntityStatementNumbers(Ent e);


    // check if a statement number is contained within the given procedure
    bool checkProcedure(Proc p, StmtNum num);

    // returns all the statement lines that are contained in the given procedure
    std::unordered_set<StmtNum> getProcedureStatementNumbers(Proc p);


    // check if given a constant, is indeed in the given statement line number
    bool checkConstant(Const c, StmtNum num);

    // returns all the statement lines that the constant appears in
    std::unordered_set<StmtNum> getConstantStatementNumbers(Const c);

private:
    PKB* pkbInstance = NULL;
    std::unordered_map <RelationshipType, StmtStmtRLStorage*> stmtStmtHandlerMap = { {RelationshipType::FOLLOWS, NULL} };
    std::unordered_map <RelationshipType, StmtEntRLStorage*> stmtEntHandlerMap = {};
    std::unordered_map <RelationshipType, EntEntRLStorage*> entEntHandlerMap = {};
        
};


