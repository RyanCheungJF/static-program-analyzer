#pragma once
#include "../../../src/unit_testing/src/stubs/CFGNodeStub.h"
#include "../../../src/utils/AppConstants.h"
#include "../../SP/AST/Node/AssignStatement.h"
#include "../../SP/Parser.h"
#include "../../SP/Tokenizer.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class CFGStorage {
public:
  void writeCFG(StmtNum num, CFGNodeStub &head);

  CFGNodeStub *getNode(StmtNum num);

private:
  std::unordered_map<StmtNum, CFGNodeStub *> procName_cfgRoot;
};