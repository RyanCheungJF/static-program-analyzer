#include <algorithm>
#include <memory>
#include <queue>
#include <set>

#include "../../qps/entities/Parameter.h"
#include "../storage/CFGStorage.h"
#include "../storage/ModifiesUsesStorage.h"
#include "../storage/ProcedureStorage.h"
#include "../storage/StmtStorage.h"

struct hashFunctionTuple {
    int cantor(int a, int b) {
        return (a + b + 1) * (a + b) / 2 + b;
    }
    int hashAlgoForTuple(int a, int b, int c) {
        return cantor(a, cantor(b, c));
    }

    size_t operator()(const std::tuple<int, int, int> x) {
        return hashAlgoForTuple(get<0>(x), get<1>(x), get<2>(x));
    }
};

struct hashFunctionAffectsT {
    size_t operator()(const std::pair<int, int>& x) const {
        return x.first * 31 + x.second;
    }
};

class AffectsHandler {
public:
    AffectsHandler(std::shared_ptr<CFGStorage> cfgStorage, std::shared_ptr<StmtStorage> stmtStorage,
                   std::shared_ptr<ProcedureStorage> procStorage, std::shared_ptr<ModifiesUsesStorage> modifiesStorage,
                   std::shared_ptr<ModifiesUsesStorage> usesStorage, bool isTransitive);
    std::vector<std::vector<std::string>> handle(Parameter param1, Parameter param2);

private:
    std::shared_ptr<CFGStorage> cfgStorage;
    std::shared_ptr<StmtStorage> stmtStorage;
    std::shared_ptr<ProcedureStorage> procStorage;
    std::shared_ptr<ModifiesUsesStorage> modifiesStorage;
    std::shared_ptr<ModifiesUsesStorage> usesStorage;
    bool isTransitive;

    // Affects(1, 2)
    std::vector<std::vector<std::string>> handleIntInt(StmtNum a1, StmtNum a2);

    // Affects(a1, 2) or Affects(_, 2)
    std::vector<std::vector<std::string>> handleWildcardInt(StmtNum a2);

    // Affects(1, a2) or Affects(1, _)
    std::vector<std::vector<std::string>> handleIntWildcard(StmtNum a1);

    // Affects(a1, a2) or Affects(_, _)
    std::vector<std::vector<std::string>> handleWildcardWildcard();

    // Affects*(1, 2)
    std::vector<std::vector<std::string>> handleIntIntTransitive(StmtNum a1, StmtNum a2); // TODO
    // Affects*(a1, 2) or Affects*(_, 2)
    std::vector<std::vector<std::string>> handleWildcardIntTransitive(StmtNum a2); // TODO
    // Affects*(1, a2) or Affects*(1, _)
    std::vector<std::vector<std::string>> handleIntWildcardTransitive(StmtNum a1); // TODO
    // Affects*(a1, a2) or Affects*(_, _)
    std::vector<std::vector<std::string>> handleWildcardWildcardTransitive(); // TODO

    // helper methods
    std::unordered_set<StmtNum> getControlFlowPathIntInt(StmtNum a1, StmtNum a2, ProcName proc);

    std::unordered_set<Ent> getVariablesModifiedInControlFlowPath(std::unordered_set<StmtNum> controlFlowPath);

    std::unordered_set<Ent> getCommonVariables(std::unordered_set<Ent> variablesModifiedInA1,
                                               std::unordered_set<Ent> variablesUsedInA2);

    std::vector<std::vector<std::string>> handleNonTransitive(Parameter param1, Parameter param2, bool isFixedIntParam1,
                                                              bool isFixedIntParam2, bool isWildCardParam1,
                                                              bool isWildCardParam2);
    std::vector<std::vector<std::string>> handleTransitive(Parameter param1, Parameter param2, bool isFixedIntParam1,
                                                           bool isFixedIntParam2, bool isWildCardParam1,
                                                           bool isWildCardParam2);
};
