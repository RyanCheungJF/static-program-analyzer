#include "CFGBuilder.h"

CFGBuilder::CFGBuilder(WritePKB* writePKB, ReadPKB* readPKB) : writePkb(writePKB), readPkb(readPKB) {}

void CFGBuilder::buildCFG(Procedure* proc) {
    buildCFGHelper(proc->statementList.get(), 0);
    writePkb->writeCFG(proc->procedureName, cfg);
    cfg = {};
}

void CFGBuilder::buildCFGHelper(StatementList* stmtList, StmtNum loopedStmtNum) {
    StmtNum lastStmtNum = stmtList->getLastStatementNumber();
    for (int i = 0; i < stmtList->statements.size(); i++) {
        StmtNum currStmtNum = stmtList->getStmtNumForStmtIdx(i);
        StmtNum nextStmtNum = currStmtNum != lastStmtNum ? stmtList->getStmtNumForStmtIdx(i + 1) : 0;

        if (const auto ifStmt = CAST_TO(IfStatement, stmtList->getStmtForStmtIdx(i))) {
            connectNodesForCFG(currStmtNum, ifStmt->getFirstStmtNumForThen());
            connectNodesForCFG(currStmtNum, ifStmt->getFirstStmtNumForElse());
            buildCFGHelper(ifStmt->thenStmtList.get(), nextStmtNum == 0 ? loopedStmtNum : nextStmtNum);
            buildCFGHelper(ifStmt->elseStmtList.get(), nextStmtNum == 0 ? loopedStmtNum : nextStmtNum);
        }
        else {
            if (nextStmtNum != 0) {
                connectNodesForCFG(currStmtNum, nextStmtNum);
            }
            else if (loopedStmtNum != 0) {
                connectNodesForCFG(currStmtNum, loopedStmtNum);
            }
        }

        if (const auto whileStmt = CAST_TO(WhileStatement, stmtList->getStmtForStmtIdx(i))) {
            connectNodesForCFG(currStmtNum, whileStmt->getFirstStmtNumForList());
            buildCFGHelper(whileStmt->stmtList.get(), currStmtNum);
        }
    }
}

void CFGBuilder::connectNodesForCFG(StmtNum curr, StmtNum next) {
    cfg[curr][AppConstants::CHILDREN].insert(next);
    cfg[next][AppConstants::PARENTS].insert(curr);
}