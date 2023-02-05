#include <unordered_map>
#include <unordered_set>

typedef int StmtNum;

class ParentStorage {
public:
    void writeParent(StmtNum parent, StmtNum child);
    bool checkParent(StmtNum parent, StmtNum child);
    std::unordered_set<StmtNum> getChildren(StmtNum parent);
    StmtNum getParent(StmtNum child);
private:
    //Each parent has a set of all their children
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> parentChildMap;

    //Each child knows their own parent
    std::unordered_map<StmtNum, StmtNum> childParentMap;
};
