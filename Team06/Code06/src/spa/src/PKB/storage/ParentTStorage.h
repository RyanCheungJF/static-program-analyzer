#include "FollowsParentStorage.h"

class ParentTStorage : public FollowsParentStorage {
public:
    void write(StmtNum parent, std::unordered_set<StmtNum> children);
};
