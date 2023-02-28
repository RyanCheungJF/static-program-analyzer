#include "ASTVisitorUtils.h"

int visitIfStatementHelper(IfStatement *ifStatement) {
  auto statementList = ifStatement->elseStmtList.get();

  if (CAST_TO(IfStatement, statementList->statements.back().get())) {
    return visitIfStatementHelper(
        CAST_TO(IfStatement, statementList->statements.back().get()));
  } else if (CAST_TO(WhileStatement, statementList->statements.back().get())) {
    return visitWhileStatementHelper(
        CAST_TO(WhileStatement, statementList->statements.back().get()));
  } else {
    return statementList->statements.back().get()->statementNumber;
  }
}

int visitWhileStatementHelper(WhileStatement *whileStatement) {
  auto statementList = whileStatement->stmtList.get();
  if (CAST_TO(IfStatement, statementList->statements.back().get())) {
    return visitIfStatementHelper(
        CAST_TO(IfStatement, statementList->statements.back().get()));
  } else if (CAST_TO(WhileStatement, statementList->statements.back().get())) {
    return visitWhileStatementHelper(
        CAST_TO(WhileStatement, statementList->statements.back().get()));
  } else {
    return statementList->statements.back().get()->statementNumber;
  }
}

void visitCondExprHelper(ConditionalExpression *e,
                         std::unordered_set<Ent> &variables,
                         std::unordered_set<int> &constants) {
  if (auto i = CAST_TO(NotConditionalExpression, e)) {
    visitCondExprHelper(i->condExpr.get(), variables, constants);
  } else if (auto i = CAST_TO(BinaryConditionalExpression, e)) {
    visitCondExprHelper(i->lhs.get(), variables, constants);
    visitCondExprHelper(i->rhs.get(), variables, constants);
  } else if (auto i = CAST_TO(RelationalExpression, e)) {
    visitExprHelper(i->lhs.get(), variables, constants);
    visitExprHelper(i->rhs.get(), variables, constants);
  }
}

void visitExprHelper(Expression *e, std::unordered_set<Ent> &variables,
                     std::unordered_set<int> &constants) {
  if (auto i = CAST_TO(MathExpression, e)) {
    visitExprHelper(i->lhs.get(), variables, constants);
    visitExprHelper(i->rhs.get(), variables, constants);
  } else if (auto i = CAST_TO(Constant, e)) {
    constants.insert(i->value);
  } else if (auto i = CAST_TO(Variable, e)) {
    variables.insert(i->name);
  }
}

void recurseStatementHelper(Statement *recurseStmt, ASTVisitor *visitor) {
  if (auto i = CAST_TO(IfStatement, recurseStmt)) {
    i->thenStmtList->accept(visitor);
    for (const auto &statement : i->thenStmtList->statements) {
      statement->accept(visitor);
      if (isContainerStatement(statement.get())) {
        recurseStatementHelper(statement.get(), visitor);
      }
    }
    i->elseStmtList->accept(visitor);
    for (const auto &statement : i->elseStmtList->statements) {
      statement->accept(visitor);
      if (isContainerStatement(statement.get())) {
        recurseStatementHelper(statement.get(), visitor);
      }
    }
  } else if (auto i = CAST_TO(WhileStatement, recurseStmt)) {
    i->stmtList->accept(visitor);
    for (const auto &statement : i->stmtList->statements) {
      statement->accept(visitor);
      if (isContainerStatement(statement.get())) {
        recurseStatementHelper(statement.get(), visitor);
      }
    }
  }
}

bool isContainerStatement(Statement *statement) {
  return CAST_TO(IfStatement, statement) || CAST_TO(WhileStatement, statement);
}

void populateRemainingTables(WritePKB *writePKB, ReadPKB *readPKB) {
  populateUsesModifies(writePKB, readPKB);
}

void populateUsesModifies(WritePKB *writePKB, ReadPKB *readPKB) {
  // First handle the call statements
  auto callStatements = readPKB->getCallStatements();
  for (auto i : callStatements) {
    /* Could be possible I handled the call statement in the recursion, so I
       check if there's anything. If one of them is empty, it means I handled it
       in the recursion. Only if both are empty, means I might not have handled
       it.*/
    if (readPKB->getUsesS(i.first).empty() &&
        readPKB->getModifiesS(i.first).empty()) {
      handleCallStmt(writePKB, readPKB, i);
    }
  }

  // Now handle the container statement (if & while)
  auto containerStatements = readPKB->getWhileStatementNumbers();
  containerStatements.merge(readPKB->getIfStatementNumbers());
  for (StmtNum i : containerStatements) {
    auto usesVariables = readPKB->getUsesS(i);
    auto modifiesVariables = readPKB->getModifiesS(i);
    auto containedStatements = readPKB->getContainedStatements(i);
    for (StmtNum j : containedStatements) {
      usesVariables.merge(readPKB->getUsesS(j));
      modifiesVariables.merge(readPKB->getModifiesS(j));
    }
    writePKB->setUsesS(i, usesVariables);
    writePKB->setModifiesS(i, modifiesVariables);
  }

  // Lastly handle the procedures
  auto procedureNames = readPKB->getAllProcedureNames();
  for (ProcName p : procedureNames) {
    auto procedureStmtNum = readPKB->getProcedureStatementNumbers(p);
    std::unordered_set<Ent> usesVariables;
    std::unordered_set<Ent> modifiesVariables;
    for (StmtNum s : procedureStmtNum) {
      usesVariables.merge(readPKB->getUsesS(s));
      modifiesVariables.merge(readPKB->getModifiesS(s));
    }
    writePKB->setUsesP(p, usesVariables);
    writePKB->setModifiesP(p, modifiesVariables);
  }
}

std::vector<std::unordered_set<Ent>>
handleCallStmt(WritePKB *writePKB, ReadPKB *readPKB,
               std::pair<StmtNum, ProcName> callStmt) {
  auto procedureStmtNum =
      readPKB->getProcedureStatementNumbers(callStmt.second);
  std::unordered_set<Ent> currUsesVariables;
  std::unordered_set<Ent> currModifiesVariables;
  for (StmtNum sn : procedureStmtNum) {
    if (readPKB->checkStatement(AppConstants::CALL, sn)) {
      std::unordered_set<Ent> moreUsesVariables;
      if (readPKB->getUsesS(readPKB->getCallStmt(sn).first).empty()) {
        moreUsesVariables =
            handleCallStmt(writePKB, readPKB, readPKB->getCallStmt(sn))[0];
      } else {
        // If I handled the call statement before, just read from it.
        moreUsesVariables = readPKB->getUsesS(readPKB->getCallStmt(sn).first);
      }
      currUsesVariables.merge(moreUsesVariables);

      std::unordered_set<Ent> moreModifiesVariables;
      if (readPKB->getModifiesS(readPKB->getCallStmt(sn).first).empty()) {
        moreModifiesVariables =
            handleCallStmt(writePKB, readPKB, readPKB->getCallStmt(sn))[1];
      } else {
        // If I handled the call statement before, just read from it.
        moreModifiesVariables =
            readPKB->getModifiesS(readPKB->getCallStmt(sn).first);
      }
      currModifiesVariables.merge(moreModifiesVariables);
    } else {
      currUsesVariables.merge(readPKB->getUsesS(sn));
      currModifiesVariables.merge(readPKB->getModifiesS(sn));
    }
  }
  writePKB->setUsesS(callStmt.first, currUsesVariables);
  writePKB->setModifiesS(callStmt.first, currModifiesVariables);
  return std::vector{currUsesVariables, currModifiesVariables};
}

void buildCFG(Procedure *proc, WritePKB *writePKB, ReadPKB *readPKB) {
  std::unordered_map<StmtNum,
                     std::pair<std::vector<StmtNum>, std::vector<StmtNum>>>
      cfg;
  buildCFGHelper(cfg, proc->statementList.get(), 0);
  auto test = readPKB->getProcedureStatementNumbers(proc->procedureName);
  // std::cout << proc->procedureName << std::endl;
  // for (StmtNum i : test) {
  //	std::cout << i << ": ";
  //	for (auto j : cfg[i].first) {
  //		std::cout << j << ", ";
  //	}
  //	std::cout << "" << std::endl;
  //}

  // for (StmtNum i : test) {
  //	std::cout << i << ": ";
  //	for (auto j : cfg[i].second) {
  //		std::cout << j << ", ";
  //	}
  //	std::cout << "" << std::endl;
  // }

  // writePKB->writeCFG(proc->procedureName, cfg);
}

void buildCFGHelper(
    std::unordered_map<
        StmtNum, std::pair<std::vector<StmtNum>, std::vector<StmtNum>>> &cfg,
    StatementList *stmtList, StmtNum loopedStmtNum) {
  StmtNum lastStmtNum = stmtList->statements.back()->statementNumber;
  for (int i = 0; i < stmtList->statements.size(); i++) {
    StmtNum currStmtNum = stmtList->statements[i]->statementNumber;
    StmtNum nextStmtNum = currStmtNum != lastStmtNum
                              ? stmtList->statements[i + 1]->statementNumber
                              : 0;

    if (auto j = CAST_TO(IfStatement, stmtList->statements[i].get())) {
      cfg[currStmtNum].first.push_back(
          j->thenStmtList.get()->statements[0]->statementNumber);
      cfg[currStmtNum].first.push_back(
          j->elseStmtList.get()->statements[0]->statementNumber);
      cfg[j->thenStmtList.get()->statements[0]->statementNumber]
          .second.push_back(currStmtNum);
      cfg[j->elseStmtList.get()->statements[0]->statementNumber]
          .second.push_back(currStmtNum);
      buildCFGHelper(cfg, j->thenStmtList.get(),
                     nextStmtNum == 0 ? loopedStmtNum : nextStmtNum);
      buildCFGHelper(cfg, j->elseStmtList.get(),
                     nextStmtNum == 0 ? loopedStmtNum : nextStmtNum);
    } else {
      if (nextStmtNum != 0) {
        cfg[currStmtNum].first.push_back(nextStmtNum);
        if (currStmtNum != 0) {
          cfg[nextStmtNum].second.push_back(currStmtNum);
        }
      } else if (loopedStmtNum != 0) {
        cfg[currStmtNum].first.push_back(loopedStmtNum);
        cfg[loopedStmtNum].second.push_back(currStmtNum);
      }
    }

    if (auto j = CAST_TO(WhileStatement, stmtList->statements[i].get())) {
      cfg[currStmtNum].first.push_back(
          j->stmtList.get()->statements[0]->statementNumber);
      cfg[j->stmtList.get()->statements[0]->statementNumber].second.push_back(
          currStmtNum);
      buildCFGHelper(cfg, j->stmtList.get(), currStmtNum);
    }
  }
}

void validateNoDuplicateProcedureName(std::vector<ProcName> procedureNames) {
  std::unordered_set<ProcName> procSet;
  for (ProcName p : procedureNames) {
    procSet.insert(p);
  }
  if (procSet.size() != procedureNames.size()) {
    throw SemanticErrorException("A program cannot have two procedures with the same name.");
  }
}

void validateCalledProceduresExist(
    std::vector<ProcName> procedureNames,
    std::unordered_map<ProcName, std::vector<ProcName>> procCallMap) {
  std::unordered_set<ProcName> calledProcedures;
  for (const auto &pair : procCallMap) {
    for (const auto &calledProc : pair.second) {
      calledProcedures.insert(calledProc);
    }
  }

  for (const auto &calledProc : calledProcedures) {
    if (std::find(procedureNames.begin(), procedureNames.end(), calledProc) ==
        procedureNames.end()) {
      throw SemanticErrorException(
          "A procedure cannot call a non-existing procedure -> " + calledProc);
    }
  }
}

void recurseCallStatementHelper(
    Statement *recurseStmt,
    std::unordered_map<ProcName, std::vector<ProcName>> &procCallMap) {
  if (auto i = CAST_TO(IfStatement, recurseStmt)) {
    for (const auto &statement : i->thenStmtList->statements) {
      if (auto i = CAST_TO(CallStatement, statement.get())) {
        procCallMap[i->parentProcedure].push_back(i->procName);
      }
      if (isContainerStatement(statement.get())) {
        recurseCallStatementHelper(statement.get(), procCallMap);
      }
    }
    for (const auto &statement : i->elseStmtList->statements) {
      if (auto i = CAST_TO(CallStatement, statement.get())) {
        procCallMap[i->parentProcedure].push_back(i->procName);
      }
      if (isContainerStatement(statement.get())) {
        recurseCallStatementHelper(statement.get(), procCallMap);
      }
    }
  } else if (auto i = CAST_TO(WhileStatement, recurseStmt)) {
    for (const auto &statement : i->stmtList->statements) {
      if (auto i = CAST_TO(CallStatement, statement.get())) {
        procCallMap[i->parentProcedure].push_back(i->procName);
      }
      if (isContainerStatement(statement.get())) {
        recurseCallStatementHelper(statement.get(), procCallMap);
      }
    }
  }
}

void validateNoCycles(
    std::vector<ProcName> procedureNames,
    std::unordered_map<ProcName, std::vector<ProcName>> procCallMap) {
  std::deque<ProcName> queue;
  std::unordered_map<ProcName, std::pair<int, std::unordered_set<ProcName>>> nodes;
  std::vector<ProcName> order;

  for (int i = 0; i < procedureNames.size(); i++) {
    nodes[procedureNames[i]] = std::make_pair(0, std::unordered_set<ProcName>());
  }

  for (const auto &pair : procCallMap) {
    ProcName callerProc = pair.first;
    for (const auto &calledProc : pair.second) {
      nodes[calledProc].first += 1;
      nodes[callerProc].second.insert(calledProc);
    }
  }

  for (const auto &pair : nodes) {
    if (pair.second.first == 0) {
      queue.push_back(pair.first);
    }
  }

  while (queue.size() > 0) {
    auto proc = queue.back();
    queue.pop_back();
    for (auto calledProc : nodes[proc].second) {
      nodes[calledProc].first -= 1;
      if (nodes[calledProc].first == 0) {
        queue.push_back(calledProc);
      }
    }
    order.push_back(proc);
  }

  if (order.size() != procedureNames.size()) {
    throw SemanticErrorException("Recursive and cyclic calls are not allowed.");
  }
}