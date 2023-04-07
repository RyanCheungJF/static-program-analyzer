#include "PKB.h"

void PKB::initializePkb() {
    this->followsStorage = std::make_shared<RelationshipStorage<StmtNum, StmtNum>>();
    this->followsTStorage = std::make_shared<RelationshipStorage<StmtNum, StmtNum>>();
    this->parentStorage = std::make_shared<RelationshipStorage<StmtNum, StmtNum>>();
    this->parentTStorage = std::make_shared<RelationshipStorage<StmtNum, StmtNum>>();
    this->statementStorage = std::make_shared<StmtStorage>();
    this->procedureStorage = std::make_shared<ProcedureStorage>();
    this->procAssignStmtStorage = std::make_shared<ProcedureStorage>();
    this->entityStorage = std::make_shared<EntityStorage<Ent>>();
    this->constantStorage = std::make_shared<EntityStorage<Const>>();
    this->assignPatternStorage = std::make_shared<PatternWithExprStorage>();
    this->ifPatternStorage = std::make_shared<PatternStorage>();
    this->whilePatternStorage = std::make_shared<PatternStorage>();
    this->callStorage = std::make_shared<CallStorage>();
    this->usesStorage = std::make_shared<ModifiesUsesStorage>();
    this->modifiesStorage = std::make_shared<ModifiesUsesStorage>();
    this->cfgStorage = std::make_shared<CFGStorage>();
    this->callsStorage = std::make_shared<RelationshipStorage<Ent, Ent>>();
    this->callsTStorage = std::make_shared<RelationshipStorage<Ent, Ent>>();

    this->relationshipCache = std::make_shared<RelationshipCache>();
    this->patternCache = std::make_shared<PatternCache>();
    this->parameterCache = std::make_shared<ParameterCache>();
    this->attributeCache = std::make_shared<AttributeCache>();
    this->comparisonCache = std::make_shared<ComparisonCache>();

    this->followsParentMap[RelationshipType::FOLLOWS] = followsStorage;
    this->followsParentMap[RelationshipType::FOLLOWST] = followsTStorage;
    this->followsParentMap[RelationshipType::PARENT] = parentStorage;
    this->followsParentMap[RelationshipType::PARENTT] = parentTStorage;

    this->modifiesUsesMap[RelationshipType::MODIFIES] = modifiesStorage;
    this->modifiesUsesMap[RelationshipType::USES] = usesStorage;

    this->ifWhilePatternMap[ParameterType::IF] = ifPatternStorage;
    this->ifWhilePatternMap[ParameterType::WHILE] = whilePatternStorage;

    this->callsMap[RelationshipType::CALLS] = callsStorage;
    this->callsMap[RelationshipType::CALLST] = callsTStorage;

    this->followsParentHandler = std::make_shared<FollowsParentHandler>(statementStorage);
    this->modifiesUsesHandler = std::make_shared<ModifiesUsesHandler>(statementStorage);
    this->callsHandler = std::make_shared<CallsHandler>();
    this->affectsHandler = std::make_shared<AffectsHandler>(cfgStorage, statementStorage, procedureStorage,
                                                            modifiesStorage, usesStorage, procAssignStmtStorage);
    this->nextHandler = std::make_shared<NextHandler>(cfgStorage, statementStorage, procedureStorage);

    this->assignPatternHandler = std::make_shared<AssignPatternHandler>(assignPatternStorage);
    this->ifWhilePatternHandler = std::make_shared<IfWhilePatternHandler>();
}

void PKB::setFollows(StmtNum followee, StmtNum follower) {
    followsStorage->write(followee, follower);
}

void PKB::setFollowsT(StmtNum followee, std::unordered_set<StmtNum>& followers) {
    followsTStorage->write(followee, followers);
}

void PKB::setParent(StmtNum parent, StmtNum children) {
    parentStorage->write(parent, children);
}

void PKB::setParentT(StmtNum parent, std::unordered_set<StmtNum>& children) {
    parentTStorage->write(parent, children);
}

void PKB::setProcedure(ProcName p, std::unordered_set<StmtNum>& lines) {
    procedureStorage->writeProcedure(p, lines);
}

void PKB::setProcAssignStmt(ProcName p, StmtNum num) {
    procAssignStmtStorage->writeProcedure(p, num);
}

void PKB::setStatement(Stmt s, StmtNum line) {
    statementStorage->writeStatement(s, line);
}

void PKB::setEntity(StmtNum num, std::unordered_set<Ent>& entities) {
    entityStorage->writeEntity(num, entities);
}

void PKB::setConstant(StmtNum num, std::unordered_set<Const>& constants) {
    constantStorage->writeEntity(num, constants);
}

void PKB::setCall(StmtNum callLine, ProcName procedure_being_called) {
    callStorage->writeCallS(callLine, procedure_being_called);
}

void PKB::setCalls(ProcName caller, std::unordered_set<ProcName>& callees) {
    callsStorage->write(caller, callees);
}

void PKB::setCallsT(ProcName caller, std::unordered_set<ProcName>& callees) {
    callsTStorage->write(caller, callees);
}

void PKB::setUsesS(StmtNum num, std::unordered_set<Ent>& entities) {
    usesStorage->write(num, entities);
}

void PKB::setUsesP(ProcName name, std::unordered_set<Ent>& entities) {
    usesStorage->write(name, entities);
}

void PKB::setModifiesS(StmtNum num, std::unordered_set<Ent>& entities) {
    modifiesStorage->write(num, entities);
}

void PKB::setModifiesP(ProcName name, std::unordered_set<Ent>& entities) {
    modifiesStorage->write(name, entities);
}

void PKB::writePattern(std::string lhs, StmtNum num, std::unique_ptr<Expression> pointer) {
    assignPatternStorage->writePattern(lhs, num, std::move(pointer));
}

void PKB::writeIfPattern(StmtNum num, std::unordered_set<Ent>& variables) {
    ifPatternStorage->writePattern(num, variables);
}

void PKB::writeWhilePattern(StmtNum num, std::unordered_set<Ent>& variables) {
    whilePatternStorage->writePattern(num, variables);
}

void PKB::writeCFG(ProcName name,
                   std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>& graph) {
    cfgStorage->writeCFG(name, graph);
}

std::vector<std::vector<std::string>> PKB::findRelationship(shared_ptr<Relationship>& rs) {
    RelationshipType type = rs->getType();
    vector<Parameter> params = rs->getParameters();
    Parameter param1 = params[0];
    Parameter param2 = params[1];

    std::vector<std::vector<std::string>> res = relationshipCache->findResult(rs);
    if (!res.empty()) {
        return res;
    }

    std::shared_ptr<RelationshipHandler> handler;
    if (followsParentMap.find(type) != followsParentMap.end()) {
        followsParentHandler->setStorage(followsParentMap.at(type));
        handler = followsParentHandler;
    }
    else if (modifiesUsesMap.find(type) != modifiesUsesMap.end()) {
        modifiesUsesHandler->setStorage(modifiesUsesMap.at(type));
        handler = modifiesUsesHandler;
    }
    else if (callsMap.find(type) != callsMap.end()) {
        callsHandler->setStorage(callsMap.at(type));
        handler = callsHandler;
    }
    else if (nextMap.find(type) != nextMap.end()) {
        handler = nextHandler;
    }
    else if (affectsMap.find(type) != affectsMap.end()) {
        handler = affectsHandler;
    }
    handler->setIsTransitive(rs->isTransitive());
    res = handler->handle(param1, param2);

    if (!res.empty()) {
        relationshipCache->addResult(rs, res);
    }
    return res;
}

std::vector<std::string> PKB::findDesignEntities(Parameter& p) {
    std::vector<std::string> res = parameterCache->findResult(p);
    if (!res.empty()) {
        return res;
    }

    if (p.isProcedureOnly()) {
        std::unordered_set<ProcName>& procs = procedureStorage->getProcNames();
        for (auto proc : procs) {
            res.push_back(proc);
        }
    }
    else if (p.isConstant()) {
        std::unordered_set<Const>& constants = constantStorage->getEntNames();
        for (auto constant : constants) {
            res.push_back(constant);
        }
    }
    else if (p.isVariable()) {
        std::unordered_set<Ent>& vars = entityStorage->getEntNames();
        for (auto var : vars) {
            res.push_back(var);
        }
    }
    else if (p.isStatementRef()) {
        std::string typeString = p.getTypeString();
        std::unordered_set<StmtNum>& stmtNums = statementStorage->getStatementNumbers(typeString);
        for (auto stmtNum : stmtNums) {
            res.push_back(to_string(stmtNum));
        }
    }

    if (!res.empty()) {
        parameterCache->addResult(p, res);
    }
    return res;
}

std::vector<std::vector<std::string>> PKB::findPattern(Pattern& p) {
    std::vector<std::vector<std::string>> res = patternCache->findResult(p);
    if (!res.empty()) {
        return res;
    }

    const Parameter& patternSyn = p.getPatternSyn();
    ParameterType patternType = p.getPatternType();

    shared_ptr<PatternHandler> handler;
    if (patternSyn.isAssign()) {
        handler = assignPatternHandler;
    }
    else if (ifWhilePatternMap.find(patternType) != ifWhilePatternMap.end()) {
        ifWhilePatternHandler->setStorage(ifWhilePatternMap.at(patternType));
        handler = ifWhilePatternHandler;
    }
    res = handler->handle(p);

    if (!res.empty()) {
        patternCache->addResult(p, res);
    }

    return res;
}

std::vector<std::vector<std::string>> PKB::findAttribute(Parameter& p) {
    AttributeType attrType = p.getAttribute();
    ParameterType paramType = p.getType();
    std::vector<std::vector<std::string>> res = attributeCache->findResult(p);
    if (!res.empty()) {
        return res;
    }

    if (p.isStatementRef()) {
        std::unordered_set<StmtNum>& stmtNums = statementStorage->getStatementNumbers(p.getTypeString());
        if (attrType == AttributeType::PROCNAME) {
            for (auto stmtNum : stmtNums) {
                std::pair<StmtNum, ProcName> numProcPair = callStorage->getCallStmt(stmtNum);
                if (numProcPair.second == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
                    continue;
                }
                res.push_back({std::to_string(stmtNum), numProcPair.second});
            }
        }
        // assumes that QPS is correct in only allowing varName for reads and prints,
        // since reads and prints will only have 1 variable tied to them
        else if (attrType == AttributeType::VARNAME) {
            for (auto stmtNum : stmtNums) {
                Ent var = *entityStorage->getEntities(stmtNum).begin();
                res.push_back({std::to_string(stmtNum), var});
            }
        }
        else if (attrType == AttributeType::STMTNO) {
            for (auto stmtNum : stmtNums) {
                res.push_back({std::to_string(stmtNum), std::to_string(stmtNum)});
            }
        }
    }
    else if (p.isConstant()) {
        std::unordered_set<Const>& consts = constantStorage->getEntNames();
        for (auto constant : consts) {
            res.push_back({constant, constant});
        }
    }
    else if (p.isVariable()) {
        std::unordered_set<Ent>& vars = entityStorage->getEntNames();
        for (auto var : vars) {
            res.push_back({var, var});
        }
    }
    else {
        std::unordered_set<ProcName>& procs = procedureStorage->getProcNames();
        for (ProcName proc : procs) {
            res.push_back({proc, proc});
        }
    }
    if (!res.empty()) {
        attributeCache->addResult(p, res);
    }
    return res;
}

std::vector<std::vector<std::string>> PKB::findWith(Comparison& c) {
    std::vector<std::vector<std::string>> res = comparisonCache->findResult(c);
    if (!res.empty()) {
        // rmb to remove
        std::cout << "comp cache hit" << endl;
        return res;
    }
    Parameter leftParam = c.getLeftParam();
    Parameter rightParam = c.getRightParam();
    bool isLeftParamFixed = leftParam.isFixedValue();
    bool isRightParamFixed = rightParam.isFixedValue();
    Ent leftParamValue = leftParam.getValue();
    Ent rightParamValue = rightParam.getValue();

    if (isLeftParamFixed) {
        if (isRightParamFixed) {
            if (leftParamValue == rightParamValue) {
                res.push_back({leftParamValue, rightParamValue});
            }
        }
        else {
            res = findAttribute(rightParam);
            filterAttributeResult(res, leftParamValue);
            for (auto& item : res) {
                std::swap(item[0], item[1]);
            }
        }
    }
    else {
        if (isRightParamFixed) {
            res = findAttribute(leftParam);
            filterAttributeResult(res, rightParamValue);
        }
        else {
            std::vector<std::vector<std::string>> leftParamRes = findAttribute(leftParam);

            std::vector<std::vector<std::string>> rightParamRes = findAttribute(rightParam);
            std::unordered_map<std::string, std::unordered_set<std::string>> tempMap;
            for (auto& pair : leftParamRes) {
                tempMap[pair[1]].insert(pair[0]);
            }
            for (auto& pair : rightParamRes) {
                for (auto& item : tempMap[pair[1]]) {
                    res.push_back({item, pair[0]});
                }
            }
        }
    }
    if (!res.empty()) {
        comparisonCache->addResult(c, res);
    }
    return res;
}

void PKB::filterAttributeResult(std::vector<std::vector<std::string>>& res, std::string val) {
    res.erase(std::remove_if(res.begin(), res.end(),
                             [&](const std::vector<std::string>& item) {
                                 return item[1] != val;
                             }),
              res.end());
}

bool PKB::checkStatement(Stmt stmt, StmtNum num) {
    return statementStorage->checkStatement(stmt, num);
}

std::unordered_set<StmtNum>& PKB::getProcedureStatementNumbers(ProcName p) {
    return procedureStorage->getProcedureStatementNumbers(p);
}

std::unordered_set<StmtNum>& PKB::getProcAssignStmtNums(ProcName p) {
    return procAssignStmtStorage->getProcedureStatementNumbers(p);
}

std::unordered_set<ProcName>& PKB::getAllProcedureNames() {
    return procedureStorage->getProcNames();
}

std::unordered_set<Ent>& PKB::getUsesS(StmtNum num) {
    return usesStorage->getRightItems(num);
}

std::unordered_set<Ent>& PKB::getUsesP(ProcName name) {
    return usesStorage->getRightItems(name);
}

std::unordered_set<Ent>& PKB::getModifiesS(StmtNum num) {
    return modifiesStorage->getRightItems(num);
}

std::unordered_set<Ent>& PKB::getModifiesP(ProcName name) {
    return modifiesStorage->getRightItems(name);
}

std::unordered_set<StmtNum>& PKB::getIfStatementNumbers() {
    return statementStorage->getStatementNumbers(IF);
}

std::unordered_set<StmtNum>& PKB::getWhileStatementNumbers() {
    return statementStorage->getStatementNumbers(WHILE);
}

std::unordered_set<StmtNum>& PKB::getContainedStatements(StmtNum containerNum) {
    return parentTStorage->getRightItems(containerNum);
}

std::pair<StmtNum, ProcName> PKB::getCallStmt(StmtNum s) {
    return callStorage->getCallStmt(s);
}

std::unordered_set<ProcName>& PKB::getCallsT(ProcName p) {
    return callsTStorage->getRightItems(p);
}

std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>& PKB::getCFG(ProcName name) {
    return cfgStorage->getGraph(name);
}

void PKB::clearCache() {
    relationshipCache->clearCache();
    parameterCache->clearCache();
    patternCache->clearCache();
    affectsHandler->clearCache();
    attributeCache->clearCache();
    comparisonCache->clearCache();
}