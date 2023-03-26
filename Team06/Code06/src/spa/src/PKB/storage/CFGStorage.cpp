#include "CFGStorage.h"

void CFGStorage::writeCFG(
    ProcName name, std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>& graph) {

    // do a deepcopy once only
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> temp;
    for (auto kv : graph) {
        StmtNum num = kv.first;
        std::unordered_map<std::string, std::unordered_set<StmtNum>> nextMap = kv.second;

        for (auto p : nextMap) {
            std::string parentOrChildren = p.first;
            std::unordered_set<StmtNum> otherNodes = p.second;

            temp[num][p.first].insert(otherNodes.begin(), otherNodes.end());
        }
    }

    proc_graph[name] = temp;
}

std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>
CFGStorage::getGraph(ProcName name) {
    if (proc_graph.find(name) == proc_graph.end()) {
        std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> empty;
        return empty;
    }
    return proc_graph[name];
}