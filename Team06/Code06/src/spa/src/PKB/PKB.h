#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "utils/AppConstants.h"
#include "storage/FollowsParentStorage.h"
#include "storage/EntityStorage.h"
#include "storage/ProcedureStorage.h"
#include "storage/StmtStorage.h"
#include "storage/PatternStorage.h"
#include "storage/ConstantStorage.h"
#include "storage/CallStorage.h"
#include "storage/ModifiesUsesStorage.h"
#include "storage/CFGStorage.h"

class PKB {
public:
    void initializePkb();

    friend class ReadPKB;
    friend class WritePKB;

private:
    std::shared_ptr<FollowsParentStorage> followsStorage;
    std::shared_ptr<FollowsParentStorage> followsTStorage;
    std::shared_ptr<FollowsParentStorage> parentStorage;
    std::shared_ptr<FollowsParentStorage> parentTStorage;
    std::shared_ptr<StmtStorage> statementStorage;
    std::shared_ptr<EntityStorage> entityStorage;
    std::shared_ptr<ProcedureStorage> procedureStorage;
    std::shared_ptr<ConstantStorage> constantStorage;
    std::shared_ptr<PatternStorage> patternStorage;
    std::shared_ptr<CallStorage> callStorage;
    std::shared_ptr<ModifiesUsesStorage> usesStorage;
    std::shared_ptr<ModifiesUsesStorage> modifiesStorage;
    std::shared_ptr<CFGStorage> cfgStorage;
};