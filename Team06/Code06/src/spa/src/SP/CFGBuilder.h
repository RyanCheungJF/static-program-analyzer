#pragma once

#include "../PKB/WritePKB.h"

class CFGBuilder {
private:
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> cfg;
    WritePKB* writeApi;

    void buildCFGHelper(StatementList* stmtList, StmtNum loopedStmtNum);
    void connectNodesForCFG(StmtNum curr, StmtNum next);

public:
    explicit CFGBuilder(WritePKB* writePKB);
    void buildCFG(Procedure* proc);
};