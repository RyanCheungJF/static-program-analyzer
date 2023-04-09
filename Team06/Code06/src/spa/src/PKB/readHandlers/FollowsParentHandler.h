#include <memory>

#include "../../QPS/entities/Parameter.h"
#include "../storage/RelationshipStorage.h"
#include "../storage/StmtStorage.h"
#include "RelationshipHandler.h"

class FollowsParentHandler : public RelationshipHandler {
public:
    FollowsParentHandler(std::shared_ptr<StmtStorage>& stmtStorage);
    std::vector<std::vector<std::string>> handle(Parameter& param1, Parameter& param2);
    void setStorage(shared_ptr<RelationshipStorage<StmtNum, StmtNum>>& rlStorage);

private:
    std::shared_ptr<RelationshipStorage<StmtNum, StmtNum>> rlStorage;
    std::shared_ptr<StmtStorage> stmtStorage;

    // e.g. FollowsT(1, 2)
    std::vector<std::vector<std::string>> handleIntInt(Parameter& param1, Parameter& param2);

    // e.g. FollowsT(1, if) OR FollowsT(a, 2)
    std::vector<std::vector<std::string>> handleOneIntOneSyn(Parameter& param1, Parameter& param2, bool isIntSyn);

    // e.g. FollowsT(1, _) OR FollowsT(_, 2)
    std::vector<std::vector<std::string>> handleOneIntOneWildcard(Parameter& param1, Parameter& param2, bool isIntWildcard);

    // e.g. FollowsT(a, while)
    std::vector<std::vector<std::string>> handleSynSyn(Parameter& param1, Parameter& param2);

    // e.g. FollowsT(a, _) OR FollowsT(_, a)
    std::vector<std::vector<std::string>> handleOneSynOneWildcard(Parameter& param1, Parameter& param2, bool isSynWildcard);

    // e.g. FollowsT(_, _)
    std::vector<std::vector<std::string>> handleWildcardWildcard();
};