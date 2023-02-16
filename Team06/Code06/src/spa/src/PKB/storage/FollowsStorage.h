#include "FollowsParentStorage.h"

class FollowsStorage : public FollowsParentStorage {
public:
    void write(StmtNum stmtNum1, StmtNum stmtNum2);
};
