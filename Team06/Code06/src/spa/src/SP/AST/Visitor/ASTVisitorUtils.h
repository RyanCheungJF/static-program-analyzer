#pragma once
#include "AST/Node/IfStatement.h"
#include "AST/Node/WhileStatement.h"

int visitIfStatementHelper(IfStatement* ifStatement);
int visitWhileStatementHelper(WhileStatement* whileStatement);