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

        auto& procedureStmtNum = readApi->getProcedureStatementNumbers(proc);

        for (const StmtNum& sn : procedureStmtNum) {
            if (readApi->checkStatement(AppConstants::CALL, sn)) {
                auto callStmt = readApi->getCallStmt(sn);
                auto& varsUsedInCalledProc = readApi->getUsesP(callStmt.second);
                auto& varsModifiedInCalledProc = readApi->getModifiesP(callStmt.second);

                writeApi->setUsesS(callStmt.first, varsUsedInCalledProc);
                writeApi->setModifiesS(callStmt.first, varsModifiedInCalledProc);

                currUsesVariables.insert(varsUsedInCalledProc.begin(), varsUsedInCalledProc.end());
                currModifiesVariables.insert(varsModifiedInCalledProc.begin(), varsModifiedInCalledProc.end());
            }
            else {
                auto& varsUsedAtStmt = readApi->getUsesS(sn);
                auto& varsModifiedAtStmt = readApi->getModifiesS(sn);

                currUsesVariables.insert(varsUsedAtStmt.begin(), varsUsedAtStmt.end());
                currModifiesVariables.insert(varsModifiedAtStmt.begin(), varsModifiedAtStmt.end());
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
        modifiesVariables.insert(readApi->getModifiesS(containerStmt).begin(),
                                 readApi->getModifiesS(containerStmt).end());

        for (StmtNum containedStmt : readApi->getContainedStatements(containerStmt)) {
            usesVariables.insert(readApi->getUsesS(containedStmt).begin(), readApi->getUsesS(containedStmt).end());
            modifiesVariables.insert(readApi->getModifiesS(containedStmt).begin(),
                                     readApi->getModifiesS(containedStmt).end());
        }
        writeApi->setUsesS(containerStmt, usesVariables);
        writeApi->setModifiesS(containerStmt, modifiesVariables);
    }
}