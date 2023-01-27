#include "Follows.h"

void Follows::setFollows(StmtNum left, StmtNum right) {
    store.writeFollows(left, right);
    return;
}

bool Follows::checkFollows(StmtNum left, StmtNum right) {
    return store.readFollows(left, right);
}