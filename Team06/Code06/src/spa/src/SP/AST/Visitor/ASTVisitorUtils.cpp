#include "ASTVisitorUtils.h"

StmtNum visitLastStatementHelper(Statement* statement) {
    if (auto ifStmt = CAST_TO(IfStatement, statement)) {
        auto statementList = ifStmt->elseStmtList.get();
        return checkLastStatementHelper(statementList);
    }
    else if (auto whileStmt = CAST_TO(WhileStatement, statement)) {
        auto statementList = whileStmt->stmtList.get();
        return checkLastStatementHelper(statementList);
    }
}

StmtNum checkLastStatementHelper(StatementList* stmtList) {
    if (CAST_TO(IfStatement, stmtList->getLastStatement())) {
        return visitLastStatementHelper(stmtList->getLastStatement());
    }
    else if (CAST_TO(WhileStatement, stmtList->getLastStatement())) {
        return visitLastStatementHelper(stmtList->getLastStatement());
    }
    else {
        return stmtList->getLastStatementNumber();
    }
}

void visitCondExprHelper(ConditionalExpression* condExpr, std::unordered_set<Ent>& variables,
                         std::unordered_set<Const>& constants) {
    if (auto notCondExpr = CAST_TO(NotConditionalExpression, condExpr)) {
        visitCondExprHelper(notCondExpr->condExpr.get(), variables, constants);
    }
    else if (auto binCondExpr = CAST_TO(BinaryConditionalExpression, condExpr)) {
        visitCondExprHelper(binCondExpr->lhs.get(), variables, constants);
        visitCondExprHelper(binCondExpr->rhs.get(), variables, constants);
    }
    else if (auto relExpr = CAST_TO(RelationalExpression, condExpr)) {
        visitExprHelper(relExpr->lhs.get(), variables, constants);
        visitExprHelper(relExpr->rhs.get(), variables, constants);
    }
}

void visitExprHelper(Expression* expr, std::unordered_set<Ent>& variables, std::unordered_set<Const>& constants) {
    if (auto mathExpr = CAST_TO(MathExpression, expr)) {
        visitExprHelper(mathExpr->lhs.get(), variables, constants);
        visitExprHelper(mathExpr->rhs.get(), variables, constants);
    }
    else if (auto constant = CAST_TO(Constant, expr)) {
        constants.insert(constant->value);
    }
    else if (auto variable = CAST_TO(Variable, expr)) {
        variables.insert(variable->name);
    }
}

void recurseStatementHelper(Statement* recurseStmt, ASTVisitor* visitor) {
    if (auto ifStmt = CAST_TO(IfStatement, recurseStmt)) {
        ifStmt->thenStmtList->accept(visitor);
        for (const auto& statement : ifStmt->getThenStatements()) {
            checkStatementHelper(statement.get(), visitor);
        }
        ifStmt->elseStmtList->accept(visitor);
        for (const auto& statement : ifStmt->getElseStatements()) {
            checkStatementHelper(statement.get(), visitor);
        }
    }
    else if (auto whileStmt = CAST_TO(WhileStatement, recurseStmt)) {
        whileStmt->stmtList->accept(visitor);
        for (const auto& statement : whileStmt->getStatements()) {
            checkStatementHelper(statement.get(), visitor);
        }
    }
}

void checkStatementHelper(Statement* recurseStmt, ASTVisitor* visitor) {
    recurseStmt->accept(visitor);
    if (isContainerStatement(recurseStmt)) {
        recurseStatementHelper(recurseStmt, visitor);
    }
}

bool isContainerStatement(Statement* statement) {
    return CAST_TO(IfStatement, statement) || CAST_TO(WhileStatement, statement);
}

void populateUsesModifies(WritePKB* writePKB, ReadPKB* readPKB) {
    processCallStatements(writePKB, readPKB);
    processContainerStatements(writePKB, readPKB);
    processProcedures(writePKB, readPKB);
}

void processCallStatements(WritePKB* writePKB, ReadPKB* readPKB) {
    auto callStatements = readPKB->getCallStatements();
    for (auto callStmt : callStatements) {
        /* Could be possible I handled the call statement in the recursion, so I
           check if there's anything. If one of them is empty, it means I handled it
           in the recursion. Only if both are empty, means I might not have handled it. */
        if (readPKB->getUsesS(callStmt.first).empty() && readPKB->getModifiesS(callStmt.first).empty()) {
            handleCallStmt(writePKB, readPKB, callStmt);
        }
    }
}

std::vector<std::unordered_set<Ent>> handleCallStmt(WritePKB* writePKB, ReadPKB* readPKB,
                                                    std::pair<StmtNum, ProcName> callStmt) {
    auto procedureStmtNum = readPKB->getProcedureStatementNumbers(callStmt.second);
    std::unordered_set<Ent> currUsesVariables;
    std::unordered_set<Ent> currModifiesVariables;
    for (StmtNum sn : procedureStmtNum) {
        if (readPKB->checkStatement(AppConstants::CALL, sn)) {
            std::unordered_set<Ent> moreUsesVariables;
            if (readPKB->getUsesS(readPKB->getCallStmt(sn).first).empty()) {
                moreUsesVariables = handleCallStmt(writePKB, readPKB, readPKB->getCallStmt(sn))[0];
            }
            else { // If I handled the call statement before, just read from it.
                moreUsesVariables = readPKB->getUsesS(readPKB->getCallStmt(sn).first);
            }
            currUsesVariables.merge(moreUsesVariables);

            std::unordered_set<Ent> moreModifiesVariables;
            if (readPKB->getModifiesS(readPKB->getCallStmt(sn).first).empty()) {
                moreModifiesVariables = handleCallStmt(writePKB, readPKB, readPKB->getCallStmt(sn))[1];
            }
            else { // If I handled the call statement before, just read from it.
                moreModifiesVariables = readPKB->getModifiesS(readPKB->getCallStmt(sn).first);
            }
            currModifiesVariables.merge(moreModifiesVariables);
        }
        else {
            currUsesVariables.merge(readPKB->getUsesS(sn));
            currModifiesVariables.merge(readPKB->getModifiesS(sn));
        }
    }
    writePKB->setUsesS(callStmt.first, currUsesVariables);
    writePKB->setModifiesS(callStmt.first, currModifiesVariables);
    return std::vector{currUsesVariables, currModifiesVariables};
}

void processContainerStatements(WritePKB* writePKB, ReadPKB* readPKB) {
    auto containerStatements = readPKB->getWhileStatementNumbers();
    containerStatements.merge(readPKB->getIfStatementNumbers());
    for (StmtNum containerStmt : containerStatements) {
        auto usesVariables = readPKB->getUsesS(containerStmt);
        auto modifiesVariables = readPKB->getModifiesS(containerStmt);
        auto containedStatements = readPKB->getContainedStatements(containerStmt);
        for (StmtNum containedStmt : containedStatements) {
            usesVariables.merge(readPKB->getUsesS(containedStmt));
            modifiesVariables.merge(readPKB->getModifiesS(containedStmt));
        }
        writePKB->setUsesS(containerStmt, usesVariables);
        writePKB->setModifiesS(containerStmt, modifiesVariables);
    }
}

void processProcedures(WritePKB* writePKB, ReadPKB* readPKB) {
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

void buildCFG(Procedure* proc, WritePKB* writePKB, ReadPKB* readPKB) {
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> cfg;
    buildCFGHelper(cfg, proc->statementList.get(), 0);
    auto test = readPKB->getProcedureStatementNumbers(proc->procedureName);
    writePKB->writeCFG(proc->procedureName, cfg);
}

void buildCFGHelper(std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>& cfg,
                    StatementList* stmtList, StmtNum loopedStmtNum) {
    StmtNum lastStmtNum = stmtList->getLastStatementNumber();
    for (int i = 0; i < stmtList->statements.size(); i++) {
        StmtNum currStmtNum = stmtList->getStmtNumForStmtIdx(i);
        StmtNum nextStmtNum = currStmtNum != lastStmtNum ? stmtList->getStmtNumForStmtIdx(i + 1) : 0;

        if (auto ifStmt = CAST_TO(IfStatement, stmtList->getStmtForStmtIdx(i))) {
            connectNodesForCFG(cfg, currStmtNum, ifStmt->getFirstStmtNumForThen());
            connectNodesForCFG(cfg, currStmtNum, ifStmt->getFirstStmtNumForElse());
            buildCFGHelper(cfg, ifStmt->thenStmtList.get(), nextStmtNum == 0 ? loopedStmtNum : nextStmtNum);
            buildCFGHelper(cfg, ifStmt->elseStmtList.get(), nextStmtNum == 0 ? loopedStmtNum : nextStmtNum);
        }
        else {
            if (nextStmtNum != 0) {
                connectNodesForCFG(cfg, currStmtNum, nextStmtNum);
            }
            else if (loopedStmtNum != 0) {
                connectNodesForCFG(cfg, currStmtNum, loopedStmtNum);
            }
        }

        if (auto whileStmt = CAST_TO(WhileStatement, stmtList->getStmtForStmtIdx(i))) {
            connectNodesForCFG(cfg, currStmtNum, whileStmt->getFirstStmtNumForList());
            buildCFGHelper(cfg, whileStmt->stmtList.get(), currStmtNum);
        }
    }
}

void connectNodesForCFG(std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>& cfg,
                        StmtNum curr, StmtNum next) {
    cfg[curr][AppConstants::CHILDREN].insert(next);
    cfg[next][AppConstants::PARENTS].insert(curr);
}

void validateNoDuplicateProcedureName(std::vector<ProcName>& procedureNames) {
    std::unordered_set<ProcName> procSet;
    for (ProcName p : procedureNames) {
        procSet.insert(p);
    }
    if (procSet.size() != procedureNames.size()) {
        throw SemanticErrorException("A program cannot have two procedures with the same name.");
    }
}

void validateCalledProceduresExist(std::vector<ProcName>& procedureNames,
                                   std::unordered_map<ProcName, std::unordered_set<ProcName>>& procCallMap) {
    std::unordered_set<ProcName> calledProcedures;
    for (const auto& pair : procCallMap) {
        for (const auto& calledProc : pair.second) {
            calledProcedures.insert(calledProc);
        }
    }

    for (const auto& calledProc : calledProcedures) {
        if (std::find(procedureNames.begin(), procedureNames.end(), calledProc) == procedureNames.end()) {
            throw SemanticErrorException("A procedure cannot call a non-existing procedure -> " + calledProc);
        }
    }
}

void recurseCallStatementHelper(Statement* recurseStmt,
                                std::unordered_map<ProcName, std::unordered_set<ProcName>>& procCallMap,
                                ProcName parentProcedure) {
    if (auto ifStmt = CAST_TO(IfStatement, recurseStmt)) {
        for (const auto& statement : ifStmt->getThenStatements()) {
            checkCallStatementHelper(statement.get(), procCallMap, parentProcedure);
        }
        for (const auto& statement : ifStmt->getElseStatements()) {
            checkCallStatementHelper(statement.get(), procCallMap, parentProcedure);
        }
    }
    else if (auto whileStmt = CAST_TO(WhileStatement, recurseStmt)) {
        for (const auto& statement : whileStmt->getStatements()) {
            checkCallStatementHelper(statement.get(), procCallMap, parentProcedure);
        }
    }
}

void checkCallStatementHelper(Statement* recurseStmt,
                              std::unordered_map<ProcName, std::unordered_set<ProcName>>& procCallMap,
                              ProcName parentProcedure) {
    if (auto callStmt = CAST_TO(CallStatement, recurseStmt)) {
        procCallMap[parentProcedure].insert(callStmt->procName);
    }
    if (isContainerStatement(recurseStmt)) {
        recurseCallStatementHelper(recurseStmt, procCallMap, parentProcedure);
    }
}

void validateNoCycles(std::vector<ProcName>& procedureNames,
                      std::unordered_map<ProcName, std::unordered_set<ProcName>>& procCallMap, WritePKB* writePkb,
                      ReadPKB* readPkb) {
    std::deque<ProcName> queue;
    std::unordered_map<ProcName, std::pair<int, std::unordered_set<ProcName>>> nodes;
    std::vector<ProcName> order;

    for (int i = 0; i < procedureNames.size(); i++) {
        nodes[procedureNames[i]] = std::make_pair(0, std::unordered_set<ProcName>());
    }

    for (const auto& pair : procCallMap) {
        ProcName callerProc = pair.first;
        for (const auto& calledProc : pair.second) {
            nodes[callerProc].first += 1;
            nodes[calledProc].second.insert(callerProc);
        }
    }

    for (const auto& pair : nodes) {
        if (pair.second.first == 0) {
            queue.push_back(pair.first);
        }
    }

    while (queue.size() > 0) {
        auto proc = queue.front();
        queue.pop_front();
        for (auto callerProc : nodes[proc].second) {
            nodes[callerProc].first -= 1;
            if (nodes[callerProc].first == 0) {
                queue.push_back(callerProc);
            }
        }
        order.push_back(proc);
    }

    if (order.size() != procedureNames.size()) {
        throw SemanticErrorException("Recursive and cyclic calls are not allowed.");
    }
    else {
        populateCallsTable(procCallMap, order, writePkb, readPkb);
    }
}

void populateCallsTable(std::unordered_map<ProcName, std::unordered_set<ProcName>>& procCallMap,
                        std::vector<ProcName>& order, WritePKB* writePKB, ReadPKB* readPKB) {
    std::unordered_set<ProcName> calleeTSet;
    for (ProcName p : order) {
        writePKB->setCalls(p, procCallMap[p]);
        for (ProcName j : procCallMap[p]) {
            calleeTSet.insert(j);
            calleeTSet.merge(readPKB->getCallsT(j));
        }
        writePKB->setCallsT(p, calleeTSet);
        calleeTSet.clear();
    }
}