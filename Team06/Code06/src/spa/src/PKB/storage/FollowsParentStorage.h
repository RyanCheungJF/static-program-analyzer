#pragma once
#include <unordered_set>
#include <unordered_map>
#include <vector>

typedef int StmtNum;

class FollowsParentStorage {
public:
    bool exists(StmtNum stmtNum1, StmtNum stmtNum2);
    std::unordered_set<StmtNum> getRightWildcard(StmtNum leftStmtNum);
    std::unordered_set<StmtNum> getLeftWildcard(StmtNum rightStmtNum);
    std::pair<std::vector<StmtNum>, std::vector<StmtNum>> getAllPairs();

protected:
    // followee to followers or parent to children map
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> leftToRightMap;

    // follower to followees or child to parents map
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> rightToLeftMap;
};