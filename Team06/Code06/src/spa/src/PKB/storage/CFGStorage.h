#pragma once
#include <stdio.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../../../src/utils/AppConstants.h"
#include "../../SP/AST/Node/AssignStatement.h"
#include "../../SP/Parser.h"
#include "../../SP/Tokenizer.h"

class CFGStorage {
public:
    //  void writeCFG(StmtNum num, CFGNodeStub &head);

    /*
     * data structure of graph is as follows:
     *
     * "proc1": {
     *     1: {parents: [], children: [2]},
     *     2: {parents: [1], children: [3]},
     *     3: {parents: [2], children: []}
     * },
     *
     * "proc2": {
     *     4: {parents: [], children: [5, 6]},
     *     5: {parents: [4], children: [7]},
     *     6: {parents: [4], children: [7]},
     *     7: {parents: [5, 6], children: []}
     * }
     *
     */

    // TODO: this could be a pointer in the future so that we do not need to do pass-by-copy
    //  and use resources on time & space
    void writeCFG(ProcName name,
                  std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph);

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> getGraph(ProcName name);

private:
    std::unordered_map<ProcName,
                       std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>>
        proc_graph;
};