#pragma once

#include "PKB.h"

//#ifndef SPA_WRITEPKB_H
//#define SPA_WRITEPKB_H
//#endif //SPA_WRITEPKB_H

typedef int StmtNum;

class WritePKB {
public:

    // Sets the pointer to the PKB instance if it is not set yet
    void setInstancePKB(PKB &pkb);

    // Sets Follows relation in PKB
    void setFollows(StmtNum left, StmtNum right);

    // Sets FollowsT relation in PKB
    void setFollowsT(StmtNum left, StmtNum right);

private:
    PKB* pkbInstance = NULL;
};
