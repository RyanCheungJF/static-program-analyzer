#pragma once

#include "../PKB/ReadPKB.h"
#include "../PKB/WritePKB.h"
#include "AST/Extractor/UsesModifiesExtractor.h"
#include "AST/Visitor/ASTVisitorUtils.h"
#include "AST/Visitor/EntRefExtractorVisitor.h"
#include "AST/Visitor/FollowsExtractorVisitor.h"
#include "AST/Visitor/ParentExtractorVisitor.h"
#include "AST/Visitor/ProcedureExtractorVisitor.h"
#include "AST/Visitor/StatementExtractorVisitor.h"
#include "CFGBuilder.h"
#include "SemanticValidator.h"

class DesignExtractor {
private:
    std::unique_ptr<Program> ASTroot;
    WritePKB* writePkb;
    ReadPKB* readPkb;

    void extractInfo(std::vector<ProcName> topoOrder);
    void extractCFG();

public:
    DesignExtractor();
    DesignExtractor(std::unique_ptr<Program> root, WritePKB* writePKB, ReadPKB* readPKB);

    void populatePKB();
};