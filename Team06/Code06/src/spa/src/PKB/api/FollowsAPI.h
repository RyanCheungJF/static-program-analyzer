//#ifndef SPA_FOLLOWSAPI_H
//#define SPA_FOLLOWSAPI_H
//#endif //SPA_FOLLOWSAPI_H

#include "../storage/FollowsStorage.h"
typedef int StmtNum;

class FollowsAPI {
public:
    void setFollows(StmtNum left, StmtNum right);
    bool checkFollows(StmtNum left, StmtNum right);
private:
    FollowsStorage store;
};
