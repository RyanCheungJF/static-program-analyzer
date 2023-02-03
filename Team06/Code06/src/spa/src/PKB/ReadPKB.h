#pragma once
#include "PKB.h"
#include "queryHandlers/StmtStmtRLQueryHandler.h"
#include "storage/Storage.h"

//#ifndef SPA_READPKB_H
//#define SPA_READPKB_H
//#endif //SPA_READPKB_H

typedef int StmtNum;

class ReadPKB {
public:

    // Sets the pointer to the PKB instance if it is not set yet
    void setInstancePKB(PKB& pkb);

    void populateMap();

    std::vector<std::pair<std::string, std::string>> findRelationship(std::string relationship);

    // Gets a Follows relation in PKB
    bool checkFollows(StmtNum left, StmtNum right);

    // Find the follower of a followee
    StmtNum getFollower(StmtNum followee);

    // Find the followee of a follower
    StmtNum getFollowee(StmtNum follower);


    // checks if a follower is in a transitive follows relationship with a followee
    bool checkFollowsT(StmtNum followee, StmtNum follower);

    // Gets followers of a followee, transitive relationship
    std::unordered_set<StmtNum> getFollowersT(StmtNum followee);

    // Gets followees of a follower, transitive relationship
    std::unordered_set<StmtNum> getFolloweesT(StmtNum follower);


    // Checks if a statement is a parent of another given their statement numbers
    bool checkParent(StmtNum parent, StmtNum child);

    // Finds all children of a statement given its statement number
    // Returns a vector of child statement numbers
    std::vector<StmtNum> getChildren(StmtNum parent);

    // Finds parent of a statement given its statement number
    StmtNum getParent(StmtNum child);


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


private:
    PKB* pkbInstance = NULL;
    std::unordered_map <std::string, std::pair<RLQueryHandler, Storage*>> queryHandlerMap;

};


