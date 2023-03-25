#pragma once

#include "SP/AST/Visitor/ASTVisitorUtils.h"

class SemanticValidator {
private:
    WritePKB* writeApi;
    ReadPKB* readApi;
    Program* ASTroot;

    std::vector<ProcName> procedureNames;
    std::unordered_map<ProcName, std::unordered_set<ProcName>> procCallMap;

    void buildCallerCalleeTable();
    void recurseCallStatementHelper(Statement* recurseStmt, ProcName parentProcedure);
    void checkCallStatementHelper(std::vector<std::unique_ptr<Statement>>& statements, ProcName parentProcedure);
    void validateNoDuplicateProcedureName();
    void validateCalledProceduresExist();
    std::vector<ProcName> validateNoCycles();
    void populateCallsTable(std::vector<ProcName>& topoOrder);

public:
    SemanticValidator(Program* root, WritePKB* writePKB, ReadPKB* readPKB);
    std::vector<ProcName> validate();
};