#include "UsesStorage.h"

//TODO: make global constants for assign, if, while, print, call, etc. standardise with SP side
void UsesStorage::writeUses(ProcedureName name, StmtType type, StmtNum num, Ent ent) {
    procName_stmtType_stmtNum[name][type].insert(num);
    stmtNum_procName_stmtType[num] = std::make_pair(name, type);
//    procedure_entities[name].insert(ent);

    if (type == "assign") {
        assign_ent[num].insert(ent);
    } else if (type == "if") {
        if_ent[num].insert(ent);
    } else if (type == "while") {
        while_ent[num].insert(ent);
    } else if (type == "print") {
        print_ent[num] = ent;
    }
}

void UsesStorage::writeUsesCall(ProcedureName caller, ProcedureName callee, StmtNum num) {
    procName_stmtType_stmtNum[caller]["call"].insert(num);
//    caller_callee[caller].insert(callee);
    call_callee[num] = callee;
    return;
}


std::unordered_set<std::string> UsesStorage::getAllEntitiesUsed(ProcedureName name) {
    std::unordered_set<std::string> seen; //TODO: check with QPS that there is no cyclic issues
    std::unordered_set<std::string> res;
    std::deque<std::string> queue;
    queue.push_back(name);

    while (!queue.empty()) {
        std::string proc = queue.front();
        queue.pop_front();
        if (seen.find(proc) != seen.end()) {
            continue;
        }

        for (auto p : procName_stmtType_stmtNum[proc]) {
            std::string type = p.first;
            auto nums = p.second;

            for (int num : nums) {
                if (type == "call") {
                    queue.push_back(call_callee[num]);
                } else if (type == "print") {
                    res.insert(print_ent[num]);
                } else if (type == "assign") {
//                    for (Ent e: assign_ent[num]) {
//                        res.insert(e);
//                    }
                    res.insert(assign_ent[num].begin(), assign_ent[num].end());
                } else if (type == "if") {
                    res.insert(if_ent[num].begin(), if_ent[num].end());
                } else if (type == "while") {
                    res.insert(while_ent[num].begin(), while_ent[num].end());
                }
            }
        }

        seen.insert(proc);
    }
    return res;
}

bool UsesStorage::checkUses(StmtNum num, Ent e) {

    // stmtNum not in source code
    if (stmtNum_procName_stmtType.find(num) == stmtNum_procName_stmtType.end()) {
        return false;
    } else if (call_callee.find(num) != call_callee.end()) { // num is a call statement
        std::string callee = call_callee[num];
        std::unordered_set<std::string> res = getAllEntitiesUsed(callee);
        return res.find(e) != res.end();
    } else {
        StmtType type = stmtNum_procName_stmtType[num].second;
        if (type == "print") {
            return print_ent[num] == e;
        } else if (type == "if") {
            return if_ent[num].find(e)  != if_ent[num].end();
        } else if (type == "while") {
            return while_ent[num].find(e)  != while_ent[num].end();
        } else if (type == "assign") {
            return assign_ent[num].find(e)  != assign_ent[num].end();
        }
        std::cout << "SHOULD NOT HIT.";
        return false; // should not hit
    }
}

bool UsesStorage::checkUses(ProcedureName name, Ent e) {
    // procedure not in source code
    if (procName_stmtType_stmtNum.find(name) == procName_stmtType_stmtNum.end()) {
        return false;
    }

    std::unordered_set<std::string> allEntities = getAllEntitiesUsed(name);
    return allEntities.find(e) != allEntities.end();
}
