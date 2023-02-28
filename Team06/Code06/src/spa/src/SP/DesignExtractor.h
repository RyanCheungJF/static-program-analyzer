#pragma once

#include "../PKB/ReadPKB.h"
#include "../PKB/WritePKB.h"
#include "AST/Visitor/ASTVisitorUtils.h"
#include "AST/Visitor/EntRefExtractorVisitor.h"
#include "AST/Visitor/FollowsExtractorVisitor.h"
#include "AST/Visitor/ParentExtractorVisitor.h"
#include "AST/Visitor/ProcedureExtractorVisitor.h"
#include "AST/Visitor/StatementExtractorVisitor.h"

class DesignExtractor {
private:
  std::unique_ptr<Program> ASTroot;
  WritePKB *writePkb;
  ReadPKB *readPkb;

  void extractInfo();
  void extractCFG();
  void validateSemantics();

public:
  DesignExtractor();
  DesignExtractor(std::unique_ptr<Program>, WritePKB *writePKB,
                  ReadPKB *readPKB);

  void populatePKB();
};