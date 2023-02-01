#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "api/Follows.h"
#include "api/FollowsT.h"
#include "api/Parent.h"
#include "api/Statement.h"

using namespace std;
typedef short PROC;

class TNode;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
public:
	static VarTable* varTable; 
	static int setProcToAST(PROC p, TNode* r);
	static TNode* getRootAST (PROC p);

    Follows* followsApi;
    FollowsT* followsTApi;
    Parent* parentApi;
    Statement* statementApi;

private:
};
