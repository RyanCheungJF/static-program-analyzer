#include "UsesModifiesExtractor.h"

UsesModifiesExtractor::UsesModifiesExtractor(WritePKB* writePKB, ReadPKB* readPKB)
    : writeApi(writePKB), readApi(readPKB) {}

void UsesModifiesExtractor::extract(std::vector<ProcName> topoOrder) {
    processProcedures(topoOrder);
    processContainerStatements();
}

void UsesModifiesExtractor::processProcedures(std::vector<ProcName> topoOrder) {
    for (ProcName proc : topoOrder) {
        auto procedureStmtNum = readApi->getProcedureStatementNumbers(proc);
        std::unordered_set<Ent> currUsesVariables;
        std::unordered_set<Ent> currModifiesVariables;

        for (StmtNum sn : procedureStmtNum) {
            if (readApi->checkStatement(AppConstants::CALL, sn)) {
                auto callStmt = readApi->getCallStmt(sn);

                std::unordered_set<Ent> temp1 = readApi->getUsesP(callStmt.second);
                writeApi->setUsesS(callStmt.first, temp1);

                std::unordered_set<Ent> temp2 = readApi->getModifiesP(callStmt.second);
                writeApi->setModifiesS(callStmt.first, temp2);
                currUsesVariables.merge(readApi->getUsesP(callStmt.second));
                currModifiesVariables.merge(readApi->getModifiesP(callStmt.second));
            }
            else {
                currUsesVariables.merge(readApi->getUsesS(sn));
                currModifiesVariables.merge(readApi->getModifiesS(sn));
            }
        }
        writeApi->setUsesP(proc, currUsesVariables);
        writeApi->setModifiesP(proc, currModifiesVariables);
    }
}

void UsesModifiesExtractor::processContainerStatements() {
    auto containerStatements = readApi->getWhileStatementNumbers();
    containerStatements.merge(readApi->getIfStatementNumbers());
    for (StmtNum containerStmt : containerStatements) {
        auto usesVariables = readApi->getUsesS(containerStmt);
        auto modifiesVariables = readApi->getModifiesS(containerStmt);
        auto containedStatements = readApi->getContainedStatements(containerStmt);
        for (StmtNum containedStmt : containedStatements) {
            usesVariables.merge(readApi->getUsesS(containedStmt));
            modifiesVariables.merge(readApi->getModifiesS(containedStmt));
        }
        writeApi->setUsesS(containerStmt, usesVariables);
        writeApi->setModifiesS(containerStmt, modifiesVariables);
    }
}