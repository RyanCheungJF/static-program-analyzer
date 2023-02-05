#include <unordered_map>

typedef int StmtNum;

class FollowsStorage {
public:
    virtual void writeFollows(StmtNum followee, StmtNum follower);
    virtual bool checkFollows(StmtNum followee, StmtNum follower);
    virtual StmtNum getFollower(StmtNum followee);
    virtual StmtNum getFollowee(StmtNum follower);
private:
    std::unordered_map<StmtNum, StmtNum> follower_followee;
    std::unordered_map<StmtNum, StmtNum> followee_follower;
};
