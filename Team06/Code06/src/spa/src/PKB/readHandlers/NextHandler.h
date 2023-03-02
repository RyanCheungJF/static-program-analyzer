#include "../../qps/entities/Parameter.h"
#include "../storage/CFGStorage.h"
#include "../storage/StmtStorage.h"
#include <algorithm>
#include <memory>


class NextHandler {
public:
    NextHandler(std::shared_ptr<CFGStorage> cfgStorage, std::shared_ptr<StmtStorage> stmtStorage);
    std::vector<std::vector<std::string>> handle(Parameter param1, Parameter param2);

private:
    std::shared_ptr<CFGStorage> cfgStorage;
    std::shared_ptr<StmtStorage> stmtStorage;

    // e.g. Next(1, 2)
    std::vector<std::vector<std::string>> handleIntInt(Parameter param1, Parameter param2);

    // e.g. Next(1, s2) or Next(1, _)
    std::vector<std::vector<std::string>> handleIntWildcard(Parameter param2);

    // e.g. Next(s1, 5) or Next(_, 5)
    std::vector<std::vector<std::string>> handleWildcardInt(Parameter param2);

    // e.g. Next(a1, 5) where a1 is assign. a1 can be replaced by other valid statement types
    std::vector<std::vector<std::string>> handleStmttypeInt(Parameter param1, Parameter param2);

    // e.g. Next(5, a1) where a1 is assign. a1 can be replaced by other valid statement types
    std::vector<std::vector<std::string>> handleIntStmttype(Parameter param1, Parameter param2);

    // e.g. Next(a1, _) where a1 is assign. a1 can be replaced by other valid statement types
    std::vector<std::vector<std::string>> handleStmttypeWildcard(Parameter param1);

    // e.g. Next(_, a1) where a1 is assign. a1 can be replaced by other valid statement types
    std::vector<std::vector<std::string>> handleWildcardStmttype(Parameter param2);

    // e.g. Next(w1, a1) where a1 is assign, w1 is while. They can be replaced by other valid statement types
    std::vector<std::vector<std::string>> handleStmttypeStmttype(Parameter param1, Parameter param2);

    // e.g. Next(s1, s2) or Next(_, _)
    std::vector<std::vector<std::string>> handleWildcardWildcard();


private:
    std::unordered_set<ParameterType> stmtTypesSet = {ParameterType::IF, ParameterType::WHILE, ParameterType::ASSIGN,
                                                   ParameterType::PRINT, ParameterType::READ, ParameterType::CALL};
};

