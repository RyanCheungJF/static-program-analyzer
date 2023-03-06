#include <algorithm>
#include <memory>
#include <queue>

#include "../../qps/entities/Parameter.h"
#include "../storage/CFGStorage.h"
#include "../storage/ProcedureStorage.h"
#include "../storage/ModifiesUsesStorage.h"
#include "../storage/StmtStorage.h"

class AffectsHandler {
public:
    AffectsHandler(std::shared_ptr<CFGStorage> cfgStorage, std::shared_ptr<StmtStorage> stmtStorage, 
                std::shared_ptr<ProcedureStorage> procStorage, std::shared_ptr<ModifiesUsesStorage> modifiesStorage,
                bool isTransitive);
    std::vector<std::vector<std::string>> handle(Parameter param1, Parameter param2);

private:
    std::shared_ptr<CFGStorage> cfgStorage;
    std::shared_ptr<StmtStorage> stmtStorage;
    std::shared_ptr<ProcedureStorage> procStorage;
    std::shared_ptr<ModifiesUsesStorage> modifiesStorage;
    bool isTransitive;

    // Affects(1, 2)
    // Affects(a1, 2) or Affects(-, 2)
    // Affects(1, a2) or Affects(1, _)
    // Affects(a1, a2) or Affects(_, _)

};
