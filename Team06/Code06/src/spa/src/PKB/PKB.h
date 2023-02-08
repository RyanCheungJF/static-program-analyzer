#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "storage/FollowsStorage.h"
#include "storage/FollowsTStorage.h"
#include "storage/ParentStorage.h"
#include "storage/EntityStorage.h"
#include "storage/ProcedureStorage.h"
#include "storage/StmtStorage.h"
#include "storage/PatternStorage.h"
#include "storage/ConstantStorage.h"

using namespace std;
typedef short PROC;

class TNode;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
public:
	static VarTable* varTable; 
	static int setProcToAST(PROC p, TNode* r);
	static TNode* getRootAST (PROC p);

    FollowsStorage* followsStorage;
    FollowsTStorage* followsTStorage;
    ParentStorage* parentStorage;
    StmtStorage* statementStorage;
    EntityStorage* entityStorage;
    ProcedureStorage* procedureStorage;
    PatternStorage* patternStorage;
    ConstantStorage* constantStorage;

private:
};
