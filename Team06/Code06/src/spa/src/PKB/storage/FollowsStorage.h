#include <unordered_map>
#include "StmtStmtRLStorage.h"

class FollowsStorage : public StmtStmtRLStorage {
public:
    void write(StmtNum stmtNum1, StmtNum stmtNum2);
    bool exists(StmtNum stmtNum1, StmtNum stmtNum2);
    std::vector<StmtNum> getRightWildcard(StmtNum followee);
    std::vector<StmtNum> getLeftWildcard(StmtNum follower);
    std::pair<std::vector<StmtNum>, std::vector<StmtNum>> getAllPairs();
private:
    std::unordered_map<StmtNum, StmtNum> followerFolloweeMap;
    std::unordered_map<StmtNum, StmtNum> followeeFollowerMap;
};
