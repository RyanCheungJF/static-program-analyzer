#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKB.h"
#include "TNode.h"

using namespace std;

void PKB::setFollows(Follows &instance) {
    if (this->followsApi != NULL) {
        return;
    }
    this->followsApi = &instance;
    return;
}

void PKB::setFollowsT(FollowsT &instance) {
    if (this->followsTApi != NULL) {
        return;
    }
    this->followsTApi = &instance;
    return;
}

void PKB::setParent(Parent &instance) {
    if (this->parentApi != NULL) {
        return;
    }
    this->parentApi = &instance;
    return;
}

void PKB::setStatement(Statement &instance) {
    if (this->statementApi != NULL) {
        return;
    }
    this->statementApi = &instance;
    return;
}

int PKB::setProcToAST(PROC p, TNode* r) {
	return 0;
}

TNode* PKB::getRootAST (PROC p){
	return nullptr;
}
