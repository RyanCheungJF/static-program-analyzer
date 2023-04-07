#include <memory>

#include "../../QPS/entities/Parameter.h"
#include "../storage/RelationshipStorage.h"
#include "RelationshipHandler.h"

class CallsHandler : public RelationshipHandler {
public:
    std::vector<std::vector<std::string>> handle(Parameter param1, Parameter param2);
    void setStorage(std::shared_ptr<RelationshipStorage<Ent, Ent>> callsStorage);

private:
    std::shared_ptr<RelationshipStorage<Ent, Ent>> callsStorage;

    // e.g. Calls("proc1", "proc2")
    std::vector<std::vector<std::string>> handleProcnameProcname(Parameter param1, Parameter param2);

    // e.g. CallsT("proc1", p2) or Calls("proc1", _)
    std::vector<std::vector<std::string>> handleProcnameWildcard(Parameter param1, bool isEarlyReturn);

    // e.g. Calls(_, "proc2") or Calls(p1, "proc2")
    std::vector<std::vector<std::string>> handleWildcardProcname(Parameter param2, bool isEarlyReturn);

    // e.g. CallsT(_, _)
    std::vector<std::vector<std::string>> handleWildcardWildcard(bool isEarlyReturn);
};
