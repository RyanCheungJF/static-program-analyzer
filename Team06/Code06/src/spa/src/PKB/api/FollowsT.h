#include "../storage/FollowsTStorage.h"
#include <unordered_set>
#include <vector>
#include <utility>
typedef int StmtNum;

class FollowsT {
public:
    virtual void writeFollowsT(std::vector<std::pair<StmtNum, StmtNum>> followee_follower);
    virtual bool checkFollowsT(StmtNum followee, StmtNum follower);
    virtual std::unordered_set<StmtNum> getFollowersT(StmtNum followee);
    virtual std::unordered_set<StmtNum> getFolloweesT(StmtNum follower);
private:
    FollowsTStorage store;
};
