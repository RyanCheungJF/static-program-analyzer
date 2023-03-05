#include "../../qps/entities/Parameter.h"
#include "../storage/CFGStorage.h"
#include "../storage/StmtStorage.h"
#include "../storage/ProcedureStorage.h"
#include <algorithm>
#include <memory>
#include <queue>

////TODO: MIGHT be buggy? would be good to have a formal proof
//struct hashFunctionNextT
//{
//    size_t operator()(const std::pair<int, int> &x) const
//    {
//        std::size_t h1 = std::hash<double>{}(x.first);
//        std::size_t h2 = std::hash<double>{}(x.second);
//        return h1 ^ h2;
//    }
//};

class NextTHandler {
public:
    NextTHandler(std::shared_ptr<CFGStorage> cfgStorage, std::shared_ptr<StmtStorage> stmtStorage, std::shared_ptr<ProcedureStorage> procStorage);
    std::vector<std::vector<std::string>> handle(Parameter param1, Parameter param2);

private:
    std::shared_ptr<CFGStorage> cfgStorage;
    std::shared_ptr<StmtStorage> stmtStorage;
    std::shared_ptr<ProcedureStorage> procStorage;
    std::unordered_map<ParameterType, std::string> stmtTypesSet = {
            {ParameterType::IF, AppConstants::IF},
            {ParameterType::WHILE, AppConstants::WHILE},
            {ParameterType::ASSIGN, AppConstants::ASSIGN},
            {ParameterType::PRINT, AppConstants::PRINT},
            {ParameterType::READ, AppConstants::READ},
            {ParameterType::CALL, AppConstants::CALL}
    };

    // e.g. Next*(1, 2)
    std::vector<std::vector<std::string>> handleIntIntTransitive(Parameter param1, Parameter param2);

    // e.g. Next*(1, s2) or Next*(1, _)
    std::vector<std::vector<std::string>> handleIntWildcardTransitive(Parameter param2);

    // e.g. Next*(s1, 5) or Next*(_, 5)
    std::vector<std::vector<std::string>> handleWildcardIntTransitive(Parameter param2);

    // e.g. Next*(a1, 5) where a1 is assign. a1 can be replaced by other valid statement types
    std::vector<std::vector<std::string>> handleStmttypeIntTransitive(Parameter param1, Parameter param2);

    // e.g. Next*(5, a1) where a1 is assign. a1 can be replaced by other valid statement types
    std::vector<std::vector<std::string>> handleIntStmttypeTransitive(Parameter param1, Parameter param2);

    // e.g. Next*(a1, _) or Next*(a1, s1) where a1 is assign. a1 can be replaced by other valid statement types
    std::vector<std::vector<std::string>> handleStmttypeWildcardTransitive(Parameter param1);

    // e.g. Next*(_, a1) or Next*(s1, a1) where a1 is assign. a1 can be replaced by other valid statement types
    std::vector<std::vector<std::string>> handleWildcardStmttypeTransitive(Parameter param2);

    // e.g. Next*(w1, a1) where a1 is assign, w1 is while. They can be replaced by other valid statement types
    std::vector<std::vector<std::string>> handleStmttypeStmttypeTransitive(Parameter param1, Parameter param2);

    // e.g. Next*(s1, s2) or Next*(_, _)
    std::vector<std::vector<std::string>> handleWildcardWildcardTransitive();

    std::unordered_map<ProcName, std::unordered_set<StmtNum>> getProcedureLines(std::unordered_set<StmtNum> statementNumbers);

};

