#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include "StmtStmtRLStorage.h"

typedef int StmtNum;

class ParentStorage : public StmtStmtRLStorage {
public:
    void write(StmtNum parent, StmtNum child);
    bool exists(StmtNum parent, StmtNum child);
    std::unordered_set<StmtNum> getRightWildcard(StmtNum parent);
    std::unordered_set<StmtNum> getLeftWildcard(StmtNum child);
    std::pair<std::vector<StmtNum>, std::vector<StmtNum>> getAllPairs();
private:
    //Each parent has a set of all their children
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> parentChildMap;

    //Each child knows their own parent
    std::unordered_map<StmtNum, StmtNum> childParentMap;
};
