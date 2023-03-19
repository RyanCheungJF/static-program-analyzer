#include "ASTVisitorUtils.h"

StmtNum visitLastStatementHelper(Statement* statement) {
    if (const auto ifStmt = CAST_TO(IfStatement, statement)) {
        auto statementList = ifStmt->elseStmtList.get();
        return checkLastStatementHelper(statementList);
    }

    if (const auto whileStmt = CAST_TO(WhileStatement, statement)) {
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
    if (const auto notCondExpr = CAST_TO(NotConditionalExpression, condExpr)) {
        visitCondExprHelper(notCondExpr->condExpr.get(), variables, constants);
    }
    else if (const auto binCondExpr = CAST_TO(BinaryConditionalExpression, condExpr)) {
        visitCondExprHelper(binCondExpr->lhs.get(), variables, constants);
        visitCondExprHelper(binCondExpr->rhs.get(), variables, constants);
    }
    else if (const auto relExpr = CAST_TO(RelationalExpression, condExpr)) {
        visitExprHelper(relExpr->lhs.get(), variables, constants);
        visitExprHelper(relExpr->rhs.get(), variables, constants);
    }
}

void visitExprHelper(Expression* expr, std::unordered_set<Ent>& variables, std::unordered_set<Const>& constants) {
    if (const auto mathExpr = CAST_TO(MathExpression, expr)) {
        visitExprHelper(mathExpr->lhs.get(), variables, constants);
        visitExprHelper(mathExpr->rhs.get(), variables, constants);
    }
    else if (const auto constant = CAST_TO(Constant, expr)) {
        constants.insert(constant->value);
    }
    else if (const auto variable = CAST_TO(Variable, expr)) {
        variables.insert(variable->name);
    }
}

void recurseStatementHelper(Statement* recurseStmt, ASTVisitor* visitor) {
    if (const auto ifStmt = CAST_TO(IfStatement, recurseStmt)) {
        ifStmt->thenStmtList->accept(visitor);
        checkStatementHelper(ifStmt->getThenStatements(), visitor);

        ifStmt->elseStmtList->accept(visitor);
        checkStatementHelper(ifStmt->getElseStatements(), visitor);
    }
    else if (const auto whileStmt = CAST_TO(WhileStatement, recurseStmt)) {
        whileStmt->stmtList->accept(visitor);
        checkStatementHelper(whileStmt->getStatements(), visitor);
    }
}

void checkStatementHelper(std::vector<std::unique_ptr<Statement>>& statements, ASTVisitor* visitor) {
    for (const auto& statement : statements) {
        statement->accept(visitor);
        if (isContainerStatement(statement.get())) {
            recurseStatementHelper(statement.get(), visitor);
        }
    }
}

bool isContainerStatement(Statement* statement) {
    return CAST_TO(IfStatement, statement) || CAST_TO(WhileStatement, statement);
}

void populateUsesModifies(WritePKB* writePKB, ReadPKB* readPKB, std::vector<ProcName> order) {
    processProcedures(writePKB, readPKB, order);
    processContainerStatements(writePKB, readPKB);
}

void processProcedures(WritePKB* writePKB, ReadPKB* readPKB, std::vector<ProcName> order) {
    for (ProcName proc : order) {
        auto procedureStmtNum = readPKB->getProcedureStatementNumbers(proc);
        std::unordered_set<Ent> currUsesVariables;
        std::unordered_set<Ent> currModifiesVariables;

        for (StmtNum sn : procedureStmtNum) {
            if (readPKB->checkStatement(AppConstants::CALL, sn)) {
                auto callStmt = readPKB->getCallStmt(sn);
                writePKB->setUsesS(callStmt.first, readPKB->getUsesP(callStmt.second));
                writePKB->setModifiesS(callStmt.first, readPKB->getModifiesP(callStmt.second));
                currUsesVariables.merge(readPKB->getUsesP(callStmt.second));
                currModifiesVariables.merge(readPKB->getModifiesP(callStmt.second));
            }
            else {
                currUsesVariables.merge(readPKB->getUsesS(sn));
                currModifiesVariables.merge(readPKB->getModifiesS(sn));
            }
        }
        writePKB->setUsesP(proc, currUsesVariables);
        writePKB->setModifiesP(proc, currModifiesVariables);
    }
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

void buildCFG(Procedure* proc, WritePKB* writePKB, ReadPKB* readPKB) {
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> cfg;
    buildCFGHelper(cfg, proc->statementList.get(), 0);
    writePKB->writeCFG(proc->procedureName, cfg);
}

void buildCFGHelper(std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>& cfg,
                    StatementList* stmtList, StmtNum loopedStmtNum) {
    StmtNum lastStmtNum = stmtList->getLastStatementNumber();
    for (int i = 0; i < stmtList->statements.size(); i++) {
        StmtNum currStmtNum = stmtList->getStmtNumForStmtIdx(i);
        StmtNum nextStmtNum = currStmtNum != lastStmtNum ? stmtList->getStmtNumForStmtIdx(i + 1) : 0;

        if (const auto ifStmt = CAST_TO(IfStatement, stmtList->getStmtForStmtIdx(i))) {
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

        if (const auto whileStmt = CAST_TO(WhileStatement, stmtList->getStmtForStmtIdx(i))) {
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
    if (procedureNames.size() > std::unordered_set<ProcName>(procedureNames.begin(), procedureNames.end()).size()) {
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
    if (const auto ifStmt = CAST_TO(IfStatement, recurseStmt)) {
        checkCallStatementHelper(ifStmt->getThenStatements(), procCallMap, parentProcedure);
        checkCallStatementHelper(ifStmt->getElseStatements(), procCallMap, parentProcedure);
    }
    else if (const auto whileStmt = CAST_TO(WhileStatement, recurseStmt)) {
        checkCallStatementHelper(whileStmt->getStatements(), procCallMap, parentProcedure);
    }
}

void checkCallStatementHelper(std::vector<std::unique_ptr<Statement>>& statements,
                              std::unordered_map<ProcName, std::unordered_set<ProcName>>& procCallMap,
                              ProcName parentProcedure) {
    for (const auto& statement : statements) {
        if (const auto callStmt = CAST_TO(CallStatement, statement.get())) {
            procCallMap[parentProcedure].insert(callStmt->procName);
        }
        if (isContainerStatement(statement.get())) {
            recurseCallStatementHelper(statement.get(), procCallMap, parentProcedure);
        }
    }
}

std::vector<ProcName> validateNoCycles(std::vector<ProcName>& procedureNames,
                                       std::unordered_map<ProcName, std::unordered_set<ProcName>>& procCallMap,
                                       WritePKB* writePkb, ReadPKB* readPkb) {
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
        ProcName proc = queue.front();
        queue.pop_front();
        for (Ent callerProc : nodes[proc].second) {
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
        return order;
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