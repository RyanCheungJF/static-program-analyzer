#pragma once

#include "../PKB/ReadPKB.h"
#include "../PKB/WritePKB.h"

class CFGBuilder {
private:
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> cfg;
    WritePKB* writePkb;
    ReadPKB* readPkb;

    void buildCFGHelper(StatementList* stmtList, StmtNum loopedStmtNum);
    void connectNodesForCFG(StmtNum curr, StmtNum next);

public:
    CFGBuilder(WritePKB* writePKB, ReadPKB* readPKB);
    void buildCFG(Procedure* proc);
};