#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "storage/FollowsStorage.h"
#include "storage/FollowsTStorage.h"
#include "storage/ParentStorage.h"
#include "storage/ParentTStorage.h"
#include "storage/EntityStorage.h"
#include "storage/ProcedureStorage.h"
#include "storage/StmtStorage.h"
#include "storage/PatternStorage.h"
#include "storage/ConstantStorage.h"
#include "storage/CallStorage.h"
#include "storage/UsesStorage.h"
#include "storage/ModifiesStorage.h"

using namespace std;

class PKB {
public:
    void initializePkb();
    
    friend class ReadPKB;
    friend class WritePKB;

private:
    std::shared_ptr<FollowsStorage> followsStorage;
    std::shared_ptr<FollowsTStorage> followsTStorage;
    std::shared_ptr<ParentStorage> parentStorage;
    std::shared_ptr<ParentTStorage> parentTStorage;
    std::shared_ptr<StmtStorage> statementStorage;
    std::shared_ptr<EntityStorage> entityStorage;
    std::shared_ptr<ProcedureStorage> procedureStorage;
    std::shared_ptr<ConstantStorage> constantStorage;
    std::shared_ptr<PatternStorage> patternStorage;
    std::shared_ptr<CallStorage> callStorage;
    std::shared_ptr<UsesStorage> usesStorage;
    std::shared_ptr<ModifiesStorage> modifiesStorage;
};
