#include <unordered_set>
#include "../../../src/spa/src/utils/AppConstants.h"

class CFGNodeStub {
public:
    std::unordered_set<StmtNum> range;
    std::unordered_set<CFGNodeStub*> parents;
    std::unordered_set<CFGNodeStub*> children;
private:

};

