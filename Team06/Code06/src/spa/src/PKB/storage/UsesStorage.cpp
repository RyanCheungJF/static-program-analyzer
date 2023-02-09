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
    call_callee[num].insert(callee);
    return;
}

bool UsesStorage::checkUses(StmtNum num, Ent e) {

    // stmtNum not in source code
    if (stmtNum_procName_stmtType.find(num) == stmtNum_procName_stmtType.end()) {
        return false;
    } else if (call_callee.find(num) != call_callee.end()) { // num is a call statement
        // TODO: what to do?
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
