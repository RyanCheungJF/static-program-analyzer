#pragma once

#include "PKB.h"

//#ifndef SPA_READPKB_H
//#define SPA_READPKB_H
//#endif //SPA_READPKB_H

typedef int StmtNum;

class ReadPKB {
public:

    // Gets a Follows relation in PKB
    bool getFollows(StmtNum left, StmtNum right);

    // Gets FollowsT relation in PKB
    bool getFollowsT(StmtNum left, StmtNum right);
};


