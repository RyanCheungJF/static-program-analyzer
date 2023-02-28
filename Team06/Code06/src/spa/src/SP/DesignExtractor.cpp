#include "DesignExtractor.h"

DesignExtractor::DesignExtractor() : ASTroot() {}

DesignExtractor::DesignExtractor(std::unique_ptr<Program> root,
                                 WritePKB *writePKB, ReadPKB *readPKB) {
  ASTroot = std::move(root);
  writePkb = writePKB;
  readPkb = readPKB;
}

void DesignExtractor::populatePKB() {
  try {
    validateSemantics();
    extractInfo();
    extractCFG();
    populateRemainingTables(writePkb, readPkb);
  } catch (SemanticErrorException e) {
    throw e;
  }
}

void DesignExtractor::validateSemantics() {
  std::vector<ProcName> procedureNames;
  std::unordered_map<ProcName, std::vector<ProcName>> procCallMap;

  for (const auto &procedure : ASTroot->procedureList) {
    procedureNames.push_back(procedure->procedureName);
    for (const auto &statement : procedure->statementList->statements) {
      if (auto i = CAST_TO(CallStatement, statement.get())) {
        procCallMap[i->parentProcedure].push_back(i->procName);
      }
      if (isContainerStatement(statement.get())) {
        recurseCallStatementHelper(statement.get(), procCallMap);
      }
    }
  }

  try {
    validateNoDuplicateProcedureName(procedureNames);
    validateCalledProceduresExist(procedureNames, procCallMap);
    validateNoCycles(procedureNames, procCallMap);
  } catch (SemanticErrorException e) {
    throw e;
  }
}

void DesignExtractor::extractInfo() {
  FollowsExtractorVisitor followsExtractor = FollowsExtractorVisitor(writePkb);
  ParentExtractorVisitor parentExtractor = ParentExtractorVisitor(writePkb);
  StatementExtractorVisitor statementExtractor =
      StatementExtractorVisitor(writePkb);
  ProcedureExtractorVisitor procedureExtractor =
      ProcedureExtractorVisitor(writePkb);
  EntRefExtractorVisitor entRefExtractor = EntRefExtractorVisitor(writePkb);
  std::vector<ASTVisitor *> visitors{&followsExtractor, &parentExtractor,
                                     &statementExtractor, &procedureExtractor,
                                     &entRefExtractor};

  for (const auto &visitor : visitors) {
    for (const auto &procedure : ASTroot->procedureList) {
      procedure->accept(visitor);
      procedure->statementList->accept(visitor);
      for (const auto &statement : procedure->statementList->statements) {
        statement->accept(visitor);
        if (isContainerStatement(statement.get())) {
          recurseStatementHelper(statement.get(), visitor);
        }
      }
    }
  }
}

void DesignExtractor::extractCFG() {
  for (const auto &procedure : ASTroot->procedureList) {
    buildCFG(procedure.get(), writePkb, readPkb);
  }
}