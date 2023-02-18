#include "FollowsParentStorage.h"

class FollowsTStorage : public FollowsParentStorage {
public:
    void write(StmtNum followee, std::unordered_set<StmtNum> followers);
};
