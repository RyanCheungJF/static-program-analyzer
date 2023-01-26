#pragma once

//#ifndef SPA_WRITEPKBAPI_H
//#define SPA_WRITEPKBAPI_H
//#endif //SPA_WRITEPKBAPI_H

typedef int StmtNum;

class WritePKBAPI {
public:
    // APIs related to Follows relation
    void SetFollows(StmtNum left, StmtNum right);

    // APIs related to Follows* relation
    void SetFollowsT(StmtNum left, StmtNum right);
};