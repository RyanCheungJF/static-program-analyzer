#include "UsesModifiesExtractor.h"

UsesModifiesExtractor::UsesModifiesExtractor(WritePKB* writePKB, ReadPKB* readPKB)
    : writeApi(writePKB), readApi(readPKB) {}

void UsesModifiesExtractor::extract(const std::vector<ProcName>& topoOrder) {
    processProcedures(topoOrder);
    processContainerStatements();
}

void UsesModifiesExtractor::processProcedures(const std::vector<ProcName>& topoOrder) {
    for (const ProcName& proc : topoOrder) {
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

            if (readApi->checkStatement(AppConstants::ASSIGN, sn)) {
                writeApi->setProcAssignStmt(proc, sn);
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

        auto& varsUsedInStmt = readApi->getUsesS(containerStmt);
        auto& varsModifiedInStmt = readApi->getModifiesS(containerStmt);
        usesVariables.insert(varsUsedInStmt.begin(), varsUsedInStmt.end());
        modifiesVariables.insert(varsModifiedInStmt.begin(), varsModifiedInStmt.end());

        for (StmtNum containedStmt : readApi->getContainedStatements(containerStmt)) {
            auto& varsUsedInContainedStmt = readApi->getUsesS(containedStmt);
            auto& varsModifiedInContainedStmt = readApi->getModifiesS(containedStmt);
            usesVariables.insert(varsUsedInContainedStmt.begin(), varsUsedInContainedStmt.end());
            modifiesVariables.insert(varsModifiedInContainedStmt.begin(), varsModifiedInContainedStmt.end());
        }
        writeApi->setUsesS(containerStmt, usesVariables);
        writeApi->setModifiesS(containerStmt, modifiesVariables);
    }
}
