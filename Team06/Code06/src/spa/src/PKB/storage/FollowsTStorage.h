#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>
#include "StmtStmtRLStorage.h"
#include <iostream>

typedef int StmtNum;

class FollowsTStorage : public StmtStmtRLStorage {
public:
    void write(StmtNum followee, std::unordered_set<StmtNum> followers);
    bool exists(StmtNum followee, StmtNum follower);
    std::unordered_set<StmtNum> getRightWildcard(StmtNum followee);
    std::unordered_set<StmtNum> getLeftWildcard(StmtNum follower);
    std::pair<std::vector<StmtNum>, std::vector<StmtNum>> getAllPairs();
private:
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> followerFolloweesMap;
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> followeeFollowersMap;
};
