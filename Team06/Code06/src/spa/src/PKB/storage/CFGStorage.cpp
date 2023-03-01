#include "CFGStorage.h"

void CFGStorage::writeCFG(StmtNum num, CFGNodeStub &head) {
  procName_cfgRoot[num] = &head;
}

CFGNodeStub *CFGStorage::getNode(StmtNum num) {
  if (procName_cfgRoot.find(num) == procName_cfgRoot.end()) {
    //        CFGNodeStub node;
    //        return &node;
    // TODO: liaise with QPS on whether they want an empty node or null
    return nullptr;
  }
  return procName_cfgRoot[num];
}
