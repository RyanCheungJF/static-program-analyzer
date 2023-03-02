#include "../../qps/entities/Parameter.h"
#include "../storage/CallStorage.h"
#include "../storage/StmtStorage.h"
#include <algorithm>
#include <memory>


class CallsHandler {
public:
    CallsHandler(std::shared_ptr<CallStorage> callStorage, std::shared_ptr<StmtStorage> stmtStorage);
    std::vector<std::vector<std::string>> handle(Parameter param1, Parameter param2);

private:
    std::shared_ptr<CallStorage> callStorage;
    std::shared_ptr<StmtStorage> stmtStorage;

    // e.g. Calls("proc1", "proc2")
    std::vector<std::vector<std::string>> handleProcnameProcname(Parameter param1, Parameter param2);

    // e.g. Calls("proc1", p2) or Calls("proc1", _)
    std::vector<std::vector<std::string>> handleProcnameWildcard(Parameter param1, Parameter param2);

    // e.g. Calls(_, "proc2") or Calls(p1, "proc2")
    std::vector<std::vector<std::string>> handleWildcardProcname(Parameter param1, Parameter param2);

    // e.g. Calls(_, _)
    std::vector<std::vector<std::string>> handleWildcardWildcard(Parameter param1, Parameter param2);

};

