#include "RelationshipStorage.h"

template <typename T, typename U> void RelationshipStorage<T, U>::write(T leftItem, U rightItem) {
    leftToRightMap[leftItem].insert(rightItem);
    rightToLeftMap[rightItem].insert(leftItem);
}

template <typename T, typename U> void RelationshipStorage<T, U>::write(T leftItem, std::unordered_set<U> rightItems) {
    leftToRightMap[leftItem].insert(rightItems.begin(), rightItems.end());
    for (U rightItem : rightItems) {
        rightToLeftMap[rightItem].insert(leftItem);
    }
}

template <typename T, typename U> bool RelationshipStorage<T, U>::exists(T leftItem, U rightItem) {
    if (leftToRightMap.find(leftItem) == leftToRightMap.end()) {
        return false;
    }
    return leftToRightMap[leftItem].find(rightItem) != leftToRightMap[leftItem].end();
}

template <typename T, typename U> std::unordered_set<U> RelationshipStorage<T, U>::getRightItems(T leftItem) {
    if (leftToRightMap.find(leftItem) == leftToRightMap.end()) {
        std::unordered_set<U> emptySet;
        return emptySet;
    }
    return leftToRightMap[leftItem];
}

template <typename T, typename U> std::unordered_set<T> RelationshipStorage<T, U>::getLeftItems(U rightItem) {
    if (rightToLeftMap.find(rightItem) == rightToLeftMap.end()) {
        std::unordered_set<T> emptySet;
        return emptySet;
    }
    return rightToLeftMap[rightItem];
}

template <typename T, typename U> std::pair<std::vector<T>, std::vector<U>> RelationshipStorage<T, U>::getAllPairs() {
    std::vector<T> allLeftItems;
    std::vector<U> allRightItems;
    for (auto row : leftToRightMap) {
        T leftItem = row.first;
        for (auto rightItem : row.second) {
            allLeftItems.push_back(leftItem);
            allRightItems.push_back(rightItem);
        }
    }
    return {allLeftItems, allRightItems};
}

template class RelationshipStorage<StmtNum, StmtNum>;
template class RelationshipStorage<StmtNum, Ent>;
template class RelationshipStorage<Ent, Ent>;