#include "UsesModifiesExtractor.h"

UsesModifiesExtractor::UsesModifiesExtractor(WritePKB* writePKB, ReadPKB* readPKB)
    : writeApi(writePKB), readApi(readPKB) {}

void UsesModifiesExtractor::extract(std::vector<ProcName> topoOrder) {
    processProcedures(topoOrder);
    processContainerStatements();
}

void UsesModifiesExtractor::processProcedures(std::vector<ProcName> topoOrder) {
    for (ProcName proc : topoOrder) {
        std::unordered_set<Ent> currUsesVariables;
        std::unordered_set<Ent> currModifiesVariables;

        for (StmtNum sn : readApi->getProcedureStatementNumbers(proc)) {
            if (readApi->checkStatement(AppConstants::CALL, sn)) {
                auto callStmt = readApi->getCallStmt(sn);

                writeApi->setUsesS(callStmt.first, readApi->getUsesP(callStmt.second));
                writeApi->setModifiesS(callStmt.first, readApi->getModifiesP(callStmt.second));

                currUsesVariables.insert(readApi->getUsesP(callStmt.second).begin(), readApi->getUsesP(callStmt.second).end());
                currModifiesVariables.insert(readApi->getModifiesP(callStmt.second).begin(), readApi->getModifiesP(callStmt.second).end());
            }
            else {
                currUsesVariables.insert(readApi->getUsesS(sn).begin(), readApi->getUsesS(sn).end());
                currModifiesVariables.insert(readApi->getModifiesS(sn).begin(), readApi->getModifiesS(sn).end());
            }
        }
        writeApi->setUsesP(proc, currUsesVariables);
        writeApi->setModifiesP(proc, currModifiesVariables);
    }
}

void UsesModifiesExtractor::processContainerStatements() {
    std::unordered_set<StmtNum> containerStatements;
    containerStatements.insert(readApi->getWhileStatementNumbers().begin(), readApi->getWhileStatementNumbers().end());
    containerStatements.insert(readApi->getIfStatementNumbers().begin(), readApi->getIfStatementNumbers().end());

    for (StmtNum containerStmt : containerStatements) {
        std::unordered_set<Ent> usesVariables;
        std::unordered_set<Ent> modifiesVariables;
        usesVariables.insert(readApi->getUsesS(containerStmt).begin(), readApi->getUsesS(containerStmt).end());
        modifiesVariables.insert(readApi->getModifiesS(containerStmt).begin(), readApi->getModifiesS(containerStmt).end());

        for (StmtNum containedStmt : readApi->getContainedStatements(containerStmt)) {
            usesVariables.insert(readApi->getUsesS(containedStmt).begin(), readApi->getUsesS(containedStmt).end());
            modifiesVariables.insert(readApi->getModifiesS(containedStmt).begin(), readApi->getModifiesS(containedStmt).end());
        }
        writeApi->setUsesS(containerStmt, usesVariables);
        writeApi->setModifiesS(containerStmt, modifiesVariables);
    }
}