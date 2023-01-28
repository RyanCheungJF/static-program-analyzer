//#ifndef SPA_FOLLOWS_H
//#define SPA_FOLLOWS_H
//#endif //SPA_FOLLOWS_H

#include "../storage/FollowsStorage.h"
typedef int StmtNum;

class Follows {
public:
    void setFollows(StmtNum left, StmtNum right);
    bool checkFollows(StmtNum left, StmtNum right);
    StmtNum getFollower(StmtNum followee);
    StmtNum getFollowee(StmtNum follower);
private:
    FollowsStorage store;
};
