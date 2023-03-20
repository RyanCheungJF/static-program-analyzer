#include <memory>
#include <queue>

#include "../../QPS/entities/Parameter.h"
#include "../storage/CFGStorage.h"
#include "../storage/ModifiesUsesStorage.h"
#include "../storage/ProcedureStorage.h"
#include "../storage/RelationshipStorage.h"
#include "../storage/StmtStorage.h"

struct hashFunctionTuple {
    int cantor(int a, int b) const {
        return (a + b + 1) * (a + b) / 2 + b;
    }
    int hashAlgoForTuple(int a, int b, int c) const {
        return cantor(a, cantor(b, c));
    }

    size_t operator()(const std::tuple<int, int, int>& x) const {
        return hashAlgoForTuple(get<0>(x), get<1>(x), get<2>(x));
    }
};

struct hashFunctionAffectsT {
    size_t operator()(const std::pair<int, int>& x) const {
        return (x.first * 501) + x.second;
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
    std::shared_ptr<RelationshipStorage<StmtNum, StmtNum>> parentTStorage;
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
    std::vector<std::vector<std::string>> handleIntIntTransitive(StmtNum a1, StmtNum a2);
    // Affects*(a1, 2) or Affects*(_, 2)
    std::vector<std::vector<std::string>> handleWildcardIntTransitive(StmtNum a2);
    // Affects*(1, a2) or Affects*(1, _)
    std::vector<std::vector<std::string>> handleIntWildcardTransitive(StmtNum a1);
    // Affects*(a1, a2) or Affects*(_, _)
    std::vector<std::vector<std::string>> handleWildcardWildcardTransitive();

    // helper methods
    std::unordered_set<StmtNum> getControlFlowPathIntInt(StmtNum a1, StmtNum a2, ProcName proc,
                                                         std::unordered_set<Ent> commonVariables);

    std::unordered_set<Ent> getCommonVariables(std::unordered_set<Ent> variablesModifiedInA1,
                                               std::unordered_set<Ent> variablesUsedInA2);

    std::vector<std::vector<std::string>> handleNonTransitive(std::string param1value, std::string param2value,
                                                              bool isFixedIntParam1, bool isFixedIntParam2,
                                                              bool isWildCardParam1, bool isWildCardParam2);
    std::vector<std::vector<std::string>> handleTransitive(std::string param1value, std::string param2value,
                                                           bool isFixedIntParam1, bool isFixedIntParam2,
                                                           bool isWildCardParam1, bool isWildCardParam2);

    std::unordered_map<StmtNum, unordered_set<StmtNum>> buildAffectsGraph(bool isInverted);

    std::unordered_set<StmtNum> getAssignStatements(std::unordered_set<StmtNum> allProcStatements);

    std::vector<std::vector<std::string>> bfsTraversalOneWildcard(StmtNum a1, StmtNum a2);

    std::vector<std::vector<std::string>> nonTransitiveOneIntOneWildcard(StmtNum a1, StmtNum a2);

    bool checkModifiedAssignReadCall(std::unordered_set<Ent> commonVariables, StmtNum currentLine);

    bool checkCanReach(StmtNum a1, StmtNum a2, ProcName proc, std::unordered_set<Ent> commonVariables);
};
