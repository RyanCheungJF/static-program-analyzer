#include "WritePKBAPI.h"

PKB pkbInstance;

void WritePKBAPI::setFollows(StmtNum left, StmtNum right) {
    pkbInstance.followsApi.setFollows(left, right);
}

void WritePKBAPI::setFollowsT(StmtNum left, StmtNum right) {

}
