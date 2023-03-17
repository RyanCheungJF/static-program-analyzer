#pragma once
#include <unordered_map>
#include <unordered_set>

#include "../../../src/utils/AppConstants.h"

template<typename T, typename U> class RelationshipStorage {
public:
    void write(T leftItem, U rightItem);
    void write(T leftItem, std::unordered_set<U> rightItems);
    bool exists(T leftItem, U rightItems);
    std::unordered_set<U> getRightItems(T leftItem);
    std::unordered_set<T> getLeftItems(U rightItem);
    std::pair<std::vector<T>, std::vector<U>> getAllPairs();

protected:
    std::unordered_map<T, std::unordered_set<U>> leftToRightMap;

    std::unordered_map<U, std::unordered_set<T>> rightToLeftMap;
};