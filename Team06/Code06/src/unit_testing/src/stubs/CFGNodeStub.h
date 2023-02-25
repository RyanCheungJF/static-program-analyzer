#include <unordered_set>
#include "../../../src/spa/src/utils/AppConstants.h"

class CFGNodeStub {
public:
    std::unordered_set<StmtNum> range;
    StmtNum first;
    StmtNum last;
    std::unordered_set<CFGNodeStub*> parents;
    std::unordered_set<CFGNodeStub*> children;
private:

};

