#include <unordered_map>
#include "StmtStmtRLStorage.h"

class FollowsStorage : public StmtStmtRLStorage {
public:
    void write(StmtNum stmtNum1, StmtNum stmtNum2);
    bool exists(StmtNum stmtNum1, StmtNum stmtNum2);
    std::vector<StmtNum> getRightWildcard(StmtNum followee);
    std::vector<StmtNum> getLeftWildcard(StmtNum follower);
private:
    std::unordered_map<StmtNum, StmtNum> follower_followee;
    std::unordered_map<StmtNum, StmtNum> followee_follower;
};
