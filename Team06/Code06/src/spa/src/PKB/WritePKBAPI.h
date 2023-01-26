#pragma once

#include "PKB.h"

//#ifndef SPA_WRITEPKBAPI_H
//#define SPA_WRITEPKBAPI_H
//#endif //SPA_WRITEPKBAPI_H

typedef int StmtNum;

class WritePKBAPI {
public:

    // Sets Follows relation in PKB
    void setFollows(StmtNum left, StmtNum right);

    // Sets FollowsT relation in PKB
    void setFollowsT(StmtNum left, StmtNum right);
};
