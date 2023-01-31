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

    void setFollows(Follows &instance);
    void setFollowsT(FollowsT &instance);
    void setParent(Parent &instance);
    void setStatement(Statement &instance);

    Follows getFollows();
    FollowsT getFollowsT();
    Parent getParent();
    Statement getStatement();

private:
    Follows* followsApi;
    FollowsT* followsTApi;
	Parent* parentApi;
    Statement* statementApi;
};
