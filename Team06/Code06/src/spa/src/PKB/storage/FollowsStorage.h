//#ifndef SPA_FOLLOWSSTORAGE_H
//#define SPA_FOLLOWSSTORAGE_H
//#endif //SPA_FOLLOWSSTORAGE_H

#include <unordered_map>

typedef int StmtNum;

class FollowsStorage {
public:
    virtual void writeFollows(StmtNum left, StmtNum right);
    virtual bool readFollows(StmtNum left, StmtNum right);
private:
    std::unordered_map<StmtNum, StmtNum> db;
};
