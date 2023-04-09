#include <memory>
#include <queue>

#include "../../QPS/entities/Parameter.h"
#include "../storage/CFGStorage.h"
#include "../storage/ProcedureStorage.h"
#include "../storage/StmtStorage.h"
#include "RelationshipHandler.h"

class NextHandler : public RelationshipHandler {
public:
    NextHandler(std::shared_ptr<CFGStorage>& cfgStorage, std::shared_ptr<StmtStorage>& stmtStorage,
                std::shared_ptr<ProcedureStorage>& procStorage);

    std::vector<std::vector<std::string>> handle(Parameter& param1, Parameter& param2);

private:
    std::shared_ptr<CFGStorage> cfgStorage;
    std::shared_ptr<StmtStorage> stmtStorage;
    std::shared_ptr<ProcedureStorage> procStorage;

    // e.g. Next(1, 2)
    std::vector<std::vector<std::string>> handleIntInt(Parameter& param1, Parameter& param2);

    // e.g. Next(1, a2) or Next(s1, 2)
    std::vector<std::vector<std::string>> oneIntOneSynNonT(Parameter& intParam, Parameter& stmtParam,
                                                           bool isFindChildren);

    // e.g. Next(1, _) or Next(_, 2)
    std::vector<std::vector<std::string>> oneIntOneWildcardNonT(Parameter& intParam, bool isFindChildren);

    // e.g. Next(a1, _) or Next(_, s2)
    std::vector<std::vector<std::string>> oneSynOneWildcardNonT(Parameter& stmtParam, bool isFindChildren);

    // e.g. Next(w1, a1) where a1 is assign, w1 is while. They can be replaced by other valid statement types
    std::vector<std::vector<std::string>> handleSynSyn(Parameter& param1, Parameter& param2);

    // e.g. Next(s1, s2) or Next(_, _)
    std::vector<std::vector<std::string>> handleWildcardWildcard();

    // e.g. Next*(1, 2)
    std::vector<std::vector<std::string>> handleIntIntTransitive(Parameter& param1, Parameter& param2);

    // Next*(a1, 2) or Next*(1, a2)
    std::vector<std::vector<std::string>> oneIntOneSynTransitive(Parameter& intParam, Parameter& stmtParam,
                                                                 bool isFindChildren);

    // e.g. Next*(1, s2) or Next*(1, _)
    std::vector<std::vector<std::string>> oneIntOneWildcardTransitive(Parameter& intParam, bool isFindChildren);

    // e.g. Next*(s1, _) or Next*(_, w2)
    std::vector<std::vector<std::string>> oneSynOneWildcardTransitive(Parameter& stmtParam, bool isFindChildren);

    // e.g. Next*(w1, a1) where a1 is assign, w1 is while. They can be replaced by other valid statement types
    std::vector<std::vector<std::string>> handleSynSynTransitive(Parameter& param1, Parameter& param2);

    // e.g. Next*(s1, s2) or Next*(_, _)
    /*std::vector<std::vector<std::string>> handleWildcardWildcardTransitive();*/

    // helper functions
    std::unordered_map<ProcName, std::unordered_set<StmtNum>>
    getProcedureLines(std::unordered_set<StmtNum>& statementNumbers);

    std::vector<std::vector<std::string>> handleNonTransitive(Parameter& param1, Parameter& param2,
                                                              bool isFixedIntParam1, bool isFixedIntParam2,
                                                              bool isWildCardParam1, bool isWildCardParam2,
                                                              bool isTypedStmtParam1, bool isTypedStmtParam2);

    std::vector<std::vector<std::string>> handleTransitive(Parameter& param1, Parameter& param2, bool isFixedIntParam1,
                                                           bool isFixedIntParam2, bool isWildCardParam1,
                                                           bool isWildCardParam2, bool isTypedStmtParam1,
                                                           bool isTypedStmtParam2);

    void addCFGRelatives(std::vector<std::vector<std::string>>& res, ProcName proc, StmtNum num, bool isFindChildren,
                         std::unordered_set<StmtNum>& filter, bool isEarlyReturn);

    void addCFGRelativesTransitive(std::vector<std::vector<std::string>>& res, const CFG& graph,
                                   std::deque<std::vector<StmtNum>>& queue, bool isFindChildren,
                                   std::unordered_set<StmtNum>& filterSet, bool isEarlyReturn);

    void initializeQueue(std::deque<std::vector<StmtNum>>& queue, const CFG& graph, StmtNum num, bool isFindChildren);

    const std::unordered_set<StmtNum>& findGraphRelative(const CFG& graph, StmtNum num, std::string relativeType);

    std::unordered_set<StmtNum> emptySet;

    inline static const bool IS_FIND_CHILDREN = true;
};
