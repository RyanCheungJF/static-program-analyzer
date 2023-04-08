#include "CFGStorage.h"

void CFGStorage::writeCFG(
    ProcName name, std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>& graph) {

    // do a deepcopy once only
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> temp;
    for (const auto &[num, nextMap] : graph) {
        for (const auto& [parentsOrChildren, neighbourNodes] : nextMap) {
            temp[num][parentsOrChildren].insert(neighbourNodes.begin(), neighbourNodes.end());
        }
    }

    proc_graph[name] = std::move(temp);
}

std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>&
CFGStorage::getGraph(ProcName name) {
    if (proc_graph.find(name) == proc_graph.end()) {
        return emptyGraph;
    }
    return proc_graph[name];
}