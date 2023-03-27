#include <memory>
#include <queue>

#include "../../qps/entities/Parameter.h"
#include "../storage/CFGStorage.h"
#include "../storage/ProcedureStorage.h"
#include "../storage/StmtStorage.h"

class NextHandler {
public:
    NextHandler(std::shared_ptr<CFGStorage> cfgStorage, std::shared_ptr<StmtStorage> stmtStorage,
                std::shared_ptr<ProcedureStorage> procStorage, bool isTransitive);
    std::vector<std::vector<std::string>> handle(Parameter param1, Parameter param2);

private:
    std::shared_ptr<CFGStorage> cfgStorage;
    std::shared_ptr<StmtStorage> stmtStorage;
    std::shared_ptr<ProcedureStorage> procStorage;
    std::unordered_map<ParameterType, std::string> stmtTypesSet = {
        {ParameterType::IF, AppConstants::IF},         {ParameterType::WHILE, AppConstants::WHILE},
        {ParameterType::ASSIGN, AppConstants::ASSIGN}, {ParameterType::PRINT, AppConstants::PRINT},
        {ParameterType::READ, AppConstants::READ},     {ParameterType::CALL, AppConstants::CALL},
        {ParameterType::STMT, AppConstants::STMT}};
    bool isTransitive;

    // e.g. Next(1, 2)
    std::vector<std::vector<std::string>> handleIntInt(Parameter param1, Parameter param2);

    // e.g. Next(1, a2) or Next(s1, 2)
    std::vector<std::vector<std::string>> oneIntOneStmtNonT(Parameter intParam, Parameter stmtParam,
                                                            bool isFindChildren);

    // e.g. Next(1, _) or Next(_, 2)
    std::vector<std::vector<std::string>> oneIntOneWildcardNonT(Parameter intParam, bool isFindChildren);

    // e.g. Next(a1, _) or Next(_, s2)
    std::vector<std::vector<std::string>> oneStmtOneWildcardNonT(Parameter stmtParam, bool isFindChildren);

    // e.g. Next(w1, a1) where a1 is assign, w1 is while. They can be replaced by other valid statement types
    std::vector<std::vector<std::string>> handleStmttypeStmttype(Parameter param1, Parameter param2);

    // e.g. Next(s1, s2) or Next(_, _)
    std::vector<std::vector<std::string>> handleWildcardWildcard();

    // e.g. Next*(1, 2)
    std::vector<std::vector<std::string>> handleIntIntTransitive(Parameter param1, Parameter param2);

    // Next*(a1, 2) or Next*(1, a2)
    std::vector<std::vector<std::string>> oneIntOneStmtTransitive(Parameter intParam, Parameter stmtParam,
                                                                  bool isFindChildren);

    // e.g. Next*(1, s2) or Next*(1, _)
    std::vector<std::vector<std::string>> oneIntOneWildcardTransitive(Parameter intParam, bool isFindChildren);

    // e.g. Next*(s1, _) or Next*(_, w2)
    std::vector<std::vector<std::string>> oneStmtOneWildcardTransitive(Parameter stmtParam, bool isFindChildren);

    // e.g. Next*(w1, a1) where a1 is assign, w1 is while. They can be replaced by other valid statement types
    std::vector<std::vector<std::string>> handleStmttypeStmttypeTransitive(Parameter param1, Parameter param2);

    // e.g. Next*(s1, s2) or Next*(_, _)
    std::vector<std::vector<std::string>> handleWildcardWildcardTransitive();

    // helper functions
    std::unordered_map<ProcName, std::unordered_set<StmtNum>>
    getProcedureLines(std::unordered_set<StmtNum>& statementNumbers);

    std::vector<std::vector<std::string>> handleNonTransitive(Parameter param1, Parameter param2, bool isFixedIntParam1,
                                                              bool isFixedIntParam2, bool isWildCardParam1,
                                                              bool isWildCardParam2, bool isTypedStmtParam1,
                                                              bool isTypedStmtParam2);

    std::vector<std::vector<std::string>> handleTransitive(Parameter param1, Parameter param2, bool isFixedIntParam1,
                                                           bool isFixedIntParam2, bool isWildCardParam1,
                                                           bool isWildCardParam2, bool isTypedStmtParam1,
                                                           bool isTypedStmtParam2);

    void addCFGRelatives(std::vector<std::vector<std::string>>& res, ProcName proc, StmtNum num, bool isFindChildren,
                         std::unordered_set<StmtNum>& filter);

    void addCFGRelativesTransitive(std::vector<std::vector<std::string>>& res, CFG& graph,
                                   std::deque<std::vector<StmtNum>>& queue, bool isFindChildren,
                                   std::unordered_set<StmtNum>& filterSet);

    void initializeQueue(std::deque<std::vector<StmtNum>>& queue, CFG& graph, StmtNum num, bool isFindChildren);
};
