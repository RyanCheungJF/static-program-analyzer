#pragma once
#include "PKB.h"

//#ifndef SPA_READPKB_H
//#define SPA_READPKB_H
//#endif //SPA_READPKB_H

typedef int StmtNum;

class ReadPKB {
public:

    // Sets the pointer to the PKB instance if it is not set yet
    void setInstancePKB(PKB &pkb);

    // Gets a Follows relation in PKB
    bool checkFollows(StmtNum left, StmtNum right);

    // Find the follower of a followee
    StmtNum getFollower(StmtNum followee);

    // Find the followee of a follower
    StmtNum getFollowee(StmtNum follower);


    // Gets FollowsT relation in PKB
    bool getFollowsT(StmtNum left, StmtNum right);


    // Checks if a statement is a parent of another given their statement numbers
    bool checkParent(StmtNum parent, StmtNum child);

    // Finds all children of a statement given its statement number
    // Returns a vector of child statement numbers
    std::vector<StmtNum> getChildren(StmtNum parent);

    // Finds parent of a statement given its statement number
    StmtNum getParent(StmtNum child);


private:
    PKB* pkbInstance = NULL;
};


