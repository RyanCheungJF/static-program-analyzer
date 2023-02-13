#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKB.h"

using namespace std;

int PKB::setProcToAST(PROC p, TNode* r) {
	return 0;
}

TNode* PKB::getRootAST (PROC p){
	return nullptr;
}

void PKB::initializePkb() {
    this->followsStorage = std::make_shared<FollowsStorage>();
    this->followsTStorage = std::make_shared<FollowsTStorage>();
    this->parentStorage = std::make_shared<ParentStorage>();
    this->statementStorage = std::make_shared<StmtStorage>();
    this->entityStorage = std::make_shared<EntityStorage>();
    this->procedureStorage = std::make_shared<ProcedureStorage>();
    this->constantStorage = std::make_shared<ConstantStorage>();
    this->patternStorage = std::make_shared<PatternStorage>();
    this->callStorage = std::make_shared<CallStorage>();
    this->usesStorage = std::make_shared<UsesStorage>();
    this->modifiesStorage = std::make_shared<ModifiesStorage>();
}
