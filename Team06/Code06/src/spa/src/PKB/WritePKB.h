#pragma once

#include "PKB.h"

//#ifndef SPA_WRITEPKB_H
//#define SPA_WRITEPKB_H
//#endif //SPA_WRITEPKB_H

typedef int StmtNum;

class WritePKB {
public:

    // Sets Follows relation in PKB
    void setFollows(StmtNum left, StmtNum right);

    // Sets FollowsT relation in PKB
    void setFollowsT(StmtNum left, StmtNum right);

//private:
    PKB* pkbInstance;
};
