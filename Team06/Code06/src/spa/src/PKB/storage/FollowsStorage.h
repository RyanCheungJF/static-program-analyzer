#include <unordered_map>

typedef int StmtNum;

class FollowsStorage {
public:
    virtual void writeFollows(StmtNum left, StmtNum right);
    virtual bool checkFollows(StmtNum left, StmtNum right);
    virtual StmtNum getFollower(StmtNum followee);
    virtual StmtNum getFollowee(StmtNum follower);
private:
    std::unordered_map<StmtNum, StmtNum> follower_followee;
    std::unordered_map<StmtNum, StmtNum> followee_follower;
};
