#include <unordered_set>
#include <vector>
#include "../../../src/spa/src/utils/AppConstants.h"

class CFGNodeStub {
public:
//    std::unordered_set<StmtNum> range;
    StmtNum first;
    StmtNum last;
    std::vector<CFGNodeStub*> parents;
    std::vector<CFGNodeStub*> children;
private:

};

