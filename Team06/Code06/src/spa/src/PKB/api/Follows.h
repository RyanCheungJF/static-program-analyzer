#include "../storage/FollowsStorage.h"
typedef int StmtNum;

class Follows {
public:
    void setFollows(StmtNum followee, StmtNum follower);
    bool checkFollows(StmtNum followee, StmtNum follower);
    StmtNum getFollower(StmtNum followee);
    StmtNum getFollowee(StmtNum follower);
private:
    FollowsStorage store;
};
