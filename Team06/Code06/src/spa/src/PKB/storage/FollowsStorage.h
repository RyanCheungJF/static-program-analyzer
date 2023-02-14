#include <unordered_map>
#include "StmtStmtRLStorage.h"

class FollowsStorage : public StmtStmtRLStorage {
public:
    void write(StmtNum stmtNum1, StmtNum stmtNum2);
    bool exists(StmtNum stmtNum1, StmtNum stmtNum2);
    std::unordered_set<StmtNum> getRightWildcard(StmtNum followee);
    std::unordered_set<StmtNum> getLeftWildcard(StmtNum follower);
    std::pair<std::vector<StmtNum>, std::vector<StmtNum>> getAllPairs();
private:
    std::unordered_map<StmtNum, StmtNum> followerFolloweeMap;
    std::unordered_map<StmtNum, StmtNum> followeeFollowerMap;
};
