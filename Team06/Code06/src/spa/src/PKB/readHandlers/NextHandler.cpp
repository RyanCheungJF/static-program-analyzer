#include "NextHandler.h"

NextHandler::NextHandler(std::shared_ptr<CFGStorage> cfgStorage, std::shared_ptr<StmtStorage> stmtStorage) {
    this->cfgStorage = cfgStorage;
    this->stmtStorage = stmtStorage;
}

