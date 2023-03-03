#include "FollowsParentStorage.h"

void FollowsParentStorage::write(StmtNum leftNum, StmtNum rightNum) {
  leftToRightMap[leftNum].insert(rightNum);
  rightToLeftMap[rightNum].insert(leftNum);
}

void FollowsParentStorage::write(StmtNum leftNum, std::unordered_set<StmtNum> rightNums) {
  leftToRightMap[leftNum].insert(rightNums.begin(), rightNums.end());
  for (StmtNum rightNum : rightNums) {
    rightToLeftMap[rightNum].insert(leftNum);
  }
}

bool FollowsParentStorage::exists(StmtNum leftNum, StmtNum rightNum) {
  if (leftToRightMap.find(leftNum) == leftToRightMap.end()) {
    return false;
  }
  return leftToRightMap[leftNum].find(rightNum) != leftToRightMap[leftNum].end();
}

std::unordered_set<StmtNum> FollowsParentStorage::getRightWildcard(StmtNum leftNum) {
  if (leftToRightMap.find(leftNum) == leftToRightMap.end()) {
    std::unordered_set<StmtNum> emptySet;
    return emptySet;
  }
  return leftToRightMap[leftNum];
}

std::unordered_set<StmtNum> FollowsParentStorage::getLeftWildcard(StmtNum rightNum) {
  if (rightToLeftMap.find(rightNum) == rightToLeftMap.end()) {
    std::unordered_set<StmtNum> emptySet;
    return emptySet;
  }
  return rightToLeftMap[rightNum];
}

std::pair<std::vector<StmtNum>, std::vector<StmtNum>> FollowsParentStorage::getAllPairs() {
  std::vector<StmtNum> allLeftNums;
  std::vector<StmtNum> allRightNums;
  for (auto row : leftToRightMap) {
    StmtNum leftNum = row.first;
    for (auto rightNum : row.second) {
      allLeftNums.push_back(leftNum);
      allRightNums.push_back(rightNum);
    }
  }
  return {allLeftNums, allRightNums};
}
