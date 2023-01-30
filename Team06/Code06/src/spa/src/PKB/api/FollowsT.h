#include "../storage/FollowsTStorage.h"
#include <unordered_set>
#include <vector>
#include <utility>
typedef int StmtNum;

class FollowsT {
public:
    void writeFollowsT(std::vector<std::pair<StmtNum, StmtNum>> followee_follower);
    bool checkFollowsT(StmtNum followee, StmtNum follower);
    std::unordered_set<StmtNum> getFollowersT(StmtNum followee);
    std::unordered_set<StmtNum> getFolloweesT(StmtNum follower);
private:
    FollowsTStorage store;
};
