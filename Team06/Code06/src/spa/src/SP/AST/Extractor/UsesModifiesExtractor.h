#pragma once

#include "../../../PKB/ReadPKB.h"
#include "../../../PKB/WritePKB.h"

class UsesModifiesExtractor {
private:
    WritePKB* writeApi;
    ReadPKB* readApi;

    void processProcedures(const std::vector<ProcName>& topoOrder);
    void processContainerStatements();

public:
    UsesModifiesExtractor(WritePKB* writePKB, ReadPKB* readPKB);
    void extract(const std::vector<ProcName>& topoOrder);
};