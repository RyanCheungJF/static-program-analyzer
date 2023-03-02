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
    // e.g. Next(1, s2) or Next(1, _)
    // e.g. Next(s1, 5) or Next(_, 5)
    // e.g. Next(a1, 5) where a1 is assign. a1 can be replaced by other valid statement types
    // e.g. Next(5, a1) where a1 is assign. a1 can be replaced by other valid statement types
    // e.g. Next(w1, a1) where a1 is assign, w1 is while. They can be replaced by other valid statement types

//    std::vector<std::vector<std::string>> handleProcnameProcname(Parameter param1, Parameter param2);
//
//    // e.g. Next(1, s2) or Calls(1, _)
//    std::vector<std::vector<std::string>> handleProcnameWildcard(Parameter param1, Parameter param2);
//
//    // e.g. Next(1, s2) or Calls(1, _)
//    std::vector<std::vector<std::string>> handleProcnameWildcard(Parameter param1, Parameter param2);
//
//    // e.g. Calls(_, "proc2") or Calls(p1, "proc2")
//    std::vector<std::vector<std::string>> handleWildcardProcname(Parameter param1, Parameter param2);
//
//    // e.g. Calls(_, _)
//    std::vector<std::vector<std::string>> handleWildcardWildcard();

};

