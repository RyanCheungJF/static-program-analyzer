#include "CFGStorage.h"

void CFGStorage::writeCFG(
    ProcName name, std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph) {
    proc_graph[name] = graph;
}

std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>
CFGStorage::getGraph(ProcName name) {
    if (proc_graph.find(name) == proc_graph.end()) {
        std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> empty;
        return empty;
    }
    return proc_graph[name];
}