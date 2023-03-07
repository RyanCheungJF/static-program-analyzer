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
    AffectsHandler(std::shared_ptr<CFGStorage> cfgStorage,
                   std::shared_ptr<StmtStorage> stmtStorage,
                   std::shared_ptr<ProcedureStorage> procStorage,
                   std::shared_ptr<ModifiesUsesStorage> modifiesStorage,
                   std::shared_ptr<ModifiesUsesStorage> usesStorage,
                   bool isTransitive);
    std::vector<std::vector<std::string>> handle(Parameter param1, Parameter param2); //todo

private:
    std::shared_ptr<CFGStorage> cfgStorage;
    std::shared_ptr<StmtStorage> stmtStorage;
    std::shared_ptr<ProcedureStorage> procStorage;
    std::shared_ptr<ModifiesUsesStorage> modifiesStorage;
    std::shared_ptr<ModifiesUsesStorage> usesStorage;
    bool isTransitive;

    // Affects(1, 2)
    std::vector<std::vector<std::string>> handleIntInt(Parameter param1, Parameter param2);

    // Affects(a1, 2) or Affects(_, 2)
    std::vector<std::vector<std::string>> handleWildcardInt(Parameter param2);

    // Affects(1, a2) or Affects(1, _)
    std::vector<std::vector<std::string>> handleIntWildcard(Parameter param1);

    // Affects(a1, a2) or Affects(_, _)
    std::vector<std::vector<std::string>> handleWildcardWildcard(); //todo

    // TODO: Affects*


    // helper methods
    std::unordered_set<StmtNum> getControlFlowPathIntInt(StmtNum a1, StmtNum a2, ProcName proc);

    std::unordered_set<Ent> getVariablesModifiedInControlFlowPath(std::unordered_set<StmtNum> controlFlowPath);

    std::unordered_set<Ent> getCommonVariables(std::unordered_set<Ent> variablesModifiedInA1, std::unordered_set<Ent> variablesUsedInA2);

};
