#pragma once
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "../../../src/utils/AppConstants.h"

class FollowsParentStorage {
public:
    void write(StmtNum leftNum, StmtNum rightNum);
    void write(StmtNum leftNum, std::unordered_set<StmtNum> rightNums);
    bool exists(StmtNum leftNum, StmtNum rightNum);
    std::unordered_set<StmtNum> getRightWildcard(StmtNum leftStmtNum);
    std::unordered_set<StmtNum> getLeftWildcard(StmtNum rightStmtNum);
    std::pair<std::vector<StmtNum>, std::vector<StmtNum>> getAllPairs();

protected:
    // followee to followers or parent to children map
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> leftToRightMap;

    // follower to followees or child to parents map
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> rightToLeftMap;
};
