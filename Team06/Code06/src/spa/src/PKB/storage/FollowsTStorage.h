#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>

typedef int StmtNum;

class FollowsTStorage {
public:
    virtual void writeFollowsT(std::vector<std::pair<StmtNum, StmtNum>> followee_follower);
    virtual bool checkFollowsT(StmtNum followee, StmtNum follower);
    virtual std::unordered_set<StmtNum> getFollowers(StmtNum followee);
    virtual std::unordered_set<StmtNum> getFollowees(StmtNum follower);
private:

    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> follower_followees;
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> followee_followers;
};
