#include "FollowsParentStorage.h"

class ParentStorage : public FollowsParentStorage {
public:
    void write(StmtNum parent, StmtNum child);
};
