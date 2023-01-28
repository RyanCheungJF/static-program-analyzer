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

private:
    PKB* pkbInstance = NULL;
};


