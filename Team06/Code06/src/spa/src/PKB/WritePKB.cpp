#include "WritePKB.h"

void WritePKB::setFollows(StmtNum left, StmtNum right) {
    pkbInstance -> followsApi.setFollows(left, right);
}

void WritePKB::setFollowsT(StmtNum left, StmtNum right) {

}
