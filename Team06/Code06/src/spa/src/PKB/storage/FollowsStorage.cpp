#include "FollowsStorage.h"

void FollowsStorage::writeFollows(StmtNum left, StmtNum right) {
    // does not exist yet
    if (db.find(left) == db.end()) {
        db.insert({ left, right});
    }
    return;
}

bool FollowsStorage::readFollows(StmtNum left, StmtNum right) {
    // does not exist yet
    if (db.find(left) == db.end()) {
        return false;
    }
    return db.at(left) == right;
}
