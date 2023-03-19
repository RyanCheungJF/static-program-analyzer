#include "PKB.h"

void PKB::initializePkb() {
    this->followsStorage = std::make_shared<RelationshipStorage<StmtNum, StmtNum>>();
    this->followsTStorage = std::make_shared<RelationshipStorage<StmtNum, StmtNum>>();
    this->parentStorage = std::make_shared<RelationshipStorage<StmtNum, StmtNum>>();
    this->parentTStorage = std::make_shared<RelationshipStorage<StmtNum, StmtNum>>();
    this->statementStorage = std::make_shared<StmtStorage>();
    this->procedureStorage = std::make_shared<ProcedureStorage>();
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
}

void PKB::setFollows(StmtNum followee, StmtNum follower) {
    followsStorage->write(followee, follower);
}

void PKB::setFollowsT(StmtNum followee, std::unordered_set<StmtNum> followers) {
    followsTStorage->write(followee, followers);
}

void PKB::setParent(StmtNum parent, StmtNum children) {
    parentStorage->write(parent, children);
}

void PKB::setParentT(StmtNum parent, std::unordered_set<StmtNum> children) {
    parentTStorage->write(parent, children);
}

void PKB::setProcedure(ProcName p, std::unordered_set<StmtNum> lines) {
    procedureStorage->writeProcedure(p, lines);
}

void PKB::setStatement(Stmt s, StmtNum line) {
    statementStorage->writeStatement(s, line);
}

void PKB::setEntity(StmtNum num, std::unordered_set<Ent> entities) {
    entityStorage->writeEntity(num, entities);
}

void PKB::setConstant(StmtNum num, std::unordered_set<Const> constants) {
    constantStorage->writeEntity(num, constants);
}

void PKB::setCall(StmtNum callLine, ProcName procedure_being_called) {
    callStorage->writeCallS(callLine, procedure_being_called);
}

void PKB::setCalls(ProcName caller, std::unordered_set<ProcName> callees) {
    callsStorage->write(caller, callees);
}

void PKB::setCallsT(ProcName caller, std::unordered_set<ProcName> callees) {
    callsTStorage->write(caller, callees);
}

void PKB::setUsesS(StmtNum num, std::unordered_set<Ent> entities) {
    usesStorage->write(num, entities);
}

void PKB::setUsesP(ProcName name, std::unordered_set<Ent> entities) {
    usesStorage->write(name, entities);
}

void PKB::setModifiesS(StmtNum num, std::unordered_set<Ent> entities) {
    modifiesStorage->write(num, entities);
}

void PKB::setModifiesP(ProcName name, std::unordered_set<Ent> entities) {
    modifiesStorage->write(name, entities);
}

void PKB::writePattern(std::string lhs, StmtNum num, std::unique_ptr<Expression> pointer) {
    assignPatternStorage->writePattern(lhs, num, std::move(pointer));
}

void PKB::writeIfPattern(StmtNum num, std::unordered_set<Ent> variables) {
    ifPatternStorage->writePattern(num, variables);
}

void PKB::writeWhilePattern(StmtNum num, std::unordered_set<Ent> variables) {
    whilePatternStorage->writePattern(num, variables);
}

void PKB::writeCFG(ProcName name,
                   std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph) {
    cfgStorage->writeCFG(name, graph);
}

std::vector<std::vector<std::string>> PKB::findRelationship(shared_ptr<Relationship> rs) {
    RelationshipType type = rs->getType();
    vector<Parameter> params = rs->getParameters();
    Parameter param1 = params[0];
    Parameter param2 = params[1];

    std::vector<std::vector<std::string>> res = relationshipCache.findResult(rs);
    if (!res.empty()) {
        return res;
    }

    if (followsParentMap.find(type) != followsParentMap.end()) {
        FollowsParentHandler handler(followsParentMap.at(type), statementStorage);
        res = handler.handle(param1, param2);
    }
    else if (modifiesUsesMap.find(type) != modifiesUsesMap.end()) {
        ModifiesUsesHandler handler(modifiesUsesMap.at(type), statementStorage);
        res = handler.handle(param1, param2);
    }
    else if (callsMap.find(type) != callsMap.end()) {
        CallsHandler handler(callsMap.at(type));
        res = handler.handle(param1, param2);
    }
    else if (nextMap.find(type) != nextMap.end()) {
        NextHandler handler(cfgStorage, statementStorage, procedureStorage, type == RelationshipType::NEXTT);
        res = handler.handle(param1, param2);
    }
    else if (affectsMap.find(type) != affectsMap.end()) {
        AffectsHandler handler(cfgStorage, statementStorage, procedureStorage, modifiesStorage, usesStorage,
                               parentTStorage, type == RelationshipType::AFFECTST);
        res = handler.handle(param1, param2);
    }
    if (!res.empty()) {
        relationshipCache.addResult(rs, res);
    }
    return res;
}

std::vector<std::string> PKB::findDesignEntities(Parameter p) {
    std::shared_ptr<Parameter> param = std::make_shared<Parameter>(p);

    std::vector<std::string> res = parameterCache.findResult(param);
    if (!res.empty()) {
        return res;
    }

    ParameterType type = p.getType();

    if (type == ParameterType::PROCEDURE) {
        std::unordered_set<ProcName> procs = procedureStorage->getProcNames();
        for (auto proc : procs) {
            res.push_back(proc);
        }
    }
    else if (type == ParameterType::CONSTANT) {
        std::unordered_set<Const> constants = constantStorage->getEntNames();
        for (auto constant : constants) {
            res.push_back(to_string(constant));
        }
    }
    else if (type == ParameterType::VARIABLE) {
        std::unordered_set<Ent> vars = entityStorage->getEntNames();
        for (auto var : vars) {
            res.push_back(var);
        }
    }
    else if (p.isStatementRef(p)) {
        std::string typeString = param->getTypeString();
        std::unordered_set<StmtNum> stmtNums = statementStorage->getStatementNumbers(typeString);
        for (auto stmtNum : stmtNums) {
            res.push_back(to_string(stmtNum));
        }
    }

    if (!res.empty()) {
        parameterCache.addResult(param, res);
    }

    return res;
}

std::vector<std::vector<std::string>> PKB::findPattern(Pattern p) {
    std::shared_ptr<Pattern> pattern = std::make_shared<Pattern>(p);
    std::vector<std::vector<std::string>> res = patternCache.findResult(pattern);
    if (!res.empty()) {
        return res;
    }

    // TODO: This violates LoD. Needs QPS to have a getPatternType() function
    ParameterType type = p.getPatternType();

    if (type == ParameterType::ASSIGN) {
        AssignPatternHandler handler(assignPatternStorage);
        res = handler.handle(p);
    }
    else if (ifWhilePatternMap.find(type) != ifWhilePatternMap.end()) {
        IfWhilePatternHandler handler(ifWhilePatternMap.at(type));
        res = handler.handle(p);
    }
    if (!res.empty()) {
        patternCache.addResult(pattern, res);
    }

    return res;
}

std::vector<std::vector<std::string>> PKB::findAttribute(With w) {
    Parameter param = w.syn;
    ParameterType paramType = param.getType();
    std::string attrType = w.attrType;
    bool hasEquals = w.hasEquals;
    std::string equalsValue = w.equalsValue;

    std::vector<std::vector<std::string>> res;

    if (Parameter::isStatementRef(param)) {
        std::unordered_set<StmtNum> stmtNums = statementStorage->getStatementNumbers(param.getTypeString());
        if (attrType == "procName") {
            for (auto stmtNum : stmtNums) {
                ProcName procName = procedureStorage->getProcedure(stmtNum);
                res.push_back({std::to_string(stmtNum), procName});
            }
        }
        // assumes that QPS is correct in only allowing varName for reads and prints,
        // since reads and prints will only have 1 variable tied to them
        else if (attrType == "varName") {
            for (auto stmtNum : stmtNums) {
                Ent var = *entityStorage->getEntities(stmtNum).begin();
                res.push_back({std::to_string(stmtNum), var});
            }
        }
        // currently just returns a pair of duplicated values. Maybe QPS can remove these trivial With clauses.
        else if (attrType == "stmtNum") {
            for (auto stmtNum : stmtNums) {
                res.push_back({std::to_string(stmtNum), std::to_string(stmtNum)});
            }
        }
    }
    // currently just returns a pair of duplicated values. Maybe QPS can remove these trivial With clauses.
    else if (paramType == ParameterType::CONSTANT) {
        std::unordered_set<Const> consts = constantStorage->getEntNames();
        for (auto constant : consts) {
            res.push_back({std::to_string(constant), std::to_string(constant)});
        }
    }
    // currently just returns a pair of duplicated values. Maybe QPS can remove these trivial With clauses.
    else if (paramType == ParameterType::VARIABLE) {
        std::unordered_set<Ent> vars = entityStorage->getEntNames();
        for (auto var : vars) {
            res.push_back({var, var});
        }
    }
    // currently just returns a pair of duplicated values. Maybe QPS can remove these trivial With clauses.
    else {
        std::unordered_set<ProcName> procs = procedureStorage->getProcNames();
        for (auto proc : procs) {
            res.push_back({proc, proc});
        }
    }

    return res;
}

bool PKB::checkStatement(Stmt stmt, StmtNum num) {
    return statementStorage->checkStatement(stmt, num);
}

std::unordered_set<StmtNum> PKB::getProcedureStatementNumbers(ProcName p) {
    return procedureStorage->getProcedureStatementNumbers(p);
}

std::unordered_set<ProcName> PKB::getAllProcedureNames() {
    return procedureStorage->getProcNames();
}

std::unordered_set<Ent> PKB::getUsesS(StmtNum num) {
    return usesStorage->getRightItems(num);
}

std::unordered_set<Ent> PKB::getUsesP(ProcName name) {
    return usesStorage->getRightItems(name);
}

std::unordered_set<Ent> PKB::getModifiesS(StmtNum num) {
    return modifiesStorage->getRightItems(num);
}

std::unordered_set<Ent> PKB::getModifiesP(ProcName name) {
    return modifiesStorage->getRightItems(name);
}

std::unordered_set<StmtNum> PKB::getIfStatementNumbers() {
    return statementStorage->getStatementNumbers(IF);
}

std::unordered_set<StmtNum> PKB::getWhileStatementNumbers() {
    return statementStorage->getStatementNumbers(WHILE);
}

std::unordered_set<StmtNum> PKB::getContainedStatements(StmtNum containerNum) {
    return parentTStorage->getRightItems(containerNum);
}

std::pair<StmtNum, ProcName> PKB::getCallStmt(StmtNum s) {
    return callStorage->getCallStmt(s);
}

std::unordered_set<ProcName> PKB::getCallsT(ProcName p) {
    return callsTStorage->getRightItems(p);
}

std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> PKB::getCFG(ProcName name) {
    return cfgStorage->getGraph(name);
}

void PKB::clearCache() {
    relationshipCache.clearCache();
    parameterCache.clearCache();
    patternCache.clearCache();
}
