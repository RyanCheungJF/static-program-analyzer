#include <memory>
#include <queue>

#include "../../QPS/entities/Parameter.h"
#include "../storage/CFGStorage.h"
#include "../storage/ModifiesUsesStorage.h"
#include "../storage/ProcedureStorage.h"
#include "../storage/RelationshipStorage.h"
#include "../storage/StmtStorage.h"
#include "RelationshipHandler.h"

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

struct hashFunctionIntInt {
    size_t operator()(const std::vector<std::string>& x) const {
        return (stoi(x[0]) * 501) + stoi(x[1]);
    }
};

class AffectsHandler : public RelationshipHandler {
public:
    AffectsHandler(std::shared_ptr<CFGStorage>& cfgStorage, std::shared_ptr<StmtStorage>& stmtStorage,
                   std::shared_ptr<ProcedureStorage>& procStorage,
                   std::shared_ptr<ModifiesUsesStorage>& modifiesStorage,
                   std::shared_ptr<ModifiesUsesStorage>& usesStorage,
                   std::shared_ptr<ProcedureStorage>& procAssignStmtStorage);

    std::vector<std::vector<std::string>> handle(Parameter& param1, Parameter& param2);

    void clearCache();

private:
    std::shared_ptr<CFGStorage> cfgStorage;
    std::shared_ptr<StmtStorage> stmtStorage;
    std::shared_ptr<ProcedureStorage> procStorage;
    std::shared_ptr<ProcedureStorage> procAssignStmtStorage;
    std::shared_ptr<ModifiesUsesStorage> modifiesStorage;
    std::shared_ptr<ModifiesUsesStorage> usesStorage;

    // handler methods
    // Affects(1, 2)
    std::vector<std::vector<std::string>> handleIntInt(StmtNum a1, StmtNum a2);
    // Affects(a1, 2) or Affects(_, 2) OR Affects(1, a2) or Affects(1, _)
    std::vector<std::vector<std::string>> handleOneIntOneWildcard(StmtNum a1, StmtNum a2, bool isEarlyReturn);
    // Affects(a1, a2) or Affects(_, _)
    std::vector<std::vector<std::string>> handleWildcardWildcard(ProcName proc, bool isEarlyReturn);

    // Affects*(1, 2)
    std::vector<std::vector<std::string>> handleIntIntTransitive(StmtNum a1, StmtNum a2);
    // Affects*(a1, 2) or Affects*(_, 2) OR Affects*(1, a2) or Affects*(1, _)
    std::vector<std::vector<std::string>> handleOneIntOneSynTransitive(StmtNum a1, StmtNum a2);
    // Affects*(a1, a2) or Affects*(_, _)
    std::vector<std::vector<std::string>> handleSynSynTransitive();

    // helper methods

    Ent getCommonVariable(std::unordered_set<Ent>& variablesModifiedInA1, std::unordered_set<Ent>& variablesUsedInA2);

    std::vector<std::vector<std::string>> handleNonTransitive(StmtNum param1Int, StmtNum param2Int,
                                                              bool isFixedIntParam1, bool isFixedIntParam2,
                                                              bool isWildCardParam1, bool isWildCardParam2,
                                                              bool isAssignStmtParam1, bool isAssignStmtParam2);
    std::vector<std::vector<std::string>> handleTransitive(StmtNum param1Int, StmtNum param2Int, bool isFixedIntParam1,
                                                           bool isFixedIntParam2, bool isWildCardParam1,
                                                           bool isWildCardParam2, bool isAssignStmtParam1,
                                                           bool isAssignStmtParam2);

    std::unordered_map<StmtNum, unordered_set<StmtNum>> buildAffectsGraph(bool isInverted, ProcName proc);

    std::vector<std::vector<std::string>> bfsTraversalOneWildcard(StmtNum a1, StmtNum a2);

    std::vector<std::vector<std::string>> nonTransitiveOneIntOneWildcard(StmtNum a1, StmtNum a2, bool isEarlyReturn);

    bool checkModifiedAssignReadCall(Ent commonVariable, StmtNum currentLine);

    bool checkCanReach(StmtNum a1, StmtNum a2, ProcName proc, Ent commonVariable);

    // cache related data structures and methods
    std::unordered_map<std::vector<std::string>, bool, hashFunctionIntInt> intIntCache;
    std::unordered_map<StmtNum, std::vector<std::vector<std::string>>> intWildcardCache;
    std::unordered_map<StmtNum, std::vector<std::vector<std::string>>> wildcardIntCache;

    std::unordered_map<std::vector<std::string>, bool, hashFunctionIntInt> intIntTransitiveCache;
    std::unordered_map<StmtNum, std::vector<std::vector<std::string>>> intWildcardTransitiveCache;
    std::unordered_map<StmtNum, std::vector<std::vector<std::string>>> wildcardIntTransitiveCache;

    std::unordered_map<ProcName, std::unordered_map<bool, std::unordered_map<StmtNum, unordered_set<StmtNum>>>>
        procAffectsGraphMap;
};
