#include "PKB.h"

void PKB::initializePkb() {
    this->followsStorage = std::make_shared<FollowsParentStorage>();
    this->followsTStorage = std::make_shared<FollowsParentStorage>();
    this->parentStorage = std::make_shared<FollowsParentStorage>();
    this->parentTStorage = std::make_shared<FollowsParentStorage>();
    this->statementStorage = std::make_shared<StmtStorage>();
    this->entityStorage = std::make_shared<EntityStorage>();
    this->procedureStorage = std::make_shared<ProcedureStorage>();
    this->constantStorage = std::make_shared<ConstantStorage>();
    this->assignPatternStorage = std::make_shared<PatternWithExprStorage>();
    this->ifPatternStorage = std::make_shared<PatternStorage>();
    this->whilePatternStorage = std::make_shared<PatternStorage>();
    this->callStorage = std::make_shared<CallStorage>();
    this->usesStorage = std::make_shared<ModifiesUsesStorage>();
    this->modifiesStorage = std::make_shared<ModifiesUsesStorage>();
    this->cfgStorage = std::make_shared<CFGStorage>();
    this->callsStorage = std::make_shared<CallsStorage>();
    this->callsTStorage = std::make_shared<CallsStorage>();

    this->followsParentMap[RelationshipType::FOLLOWS] = followsStorage;
    this->followsParentMap[RelationshipType::FOLLOWST] = followsTStorage;
    this->followsParentMap[RelationshipType::PARENT] = parentStorage;
    this->followsParentMap[RelationshipType::PARENTT] = parentTStorage;

    this->modifiesUsesMap[RelationshipType::MODIFIES] = modifiesStorage;
    this->modifiesUsesMap[RelationshipType::USES] = usesStorage;

    this->ifWhilePatternMap[ParameterType::IF] = ifPatternStorage;
    this->ifWhilePatternMap[ParameterType::WHILE] = whilePatternStorage;
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
    constantStorage->writeConstant(num, constants);
}

void PKB::setCall(StmtNum callLine, ProcName procedure_being_called) {
    callStorage->writeCallS(callLine, procedure_being_called);
}

void PKB::setCalls(ProcName caller, std::unordered_set<ProcName> callees) {
    callsStorage->writeCallP(caller, callees);
}

void PKB::setCallsT(ProcName caller, std::unordered_set<ProcName> callees) {
    callsStorage->writeCallP(caller, callees);
}

void PKB::setUsesS(StmtNum num, std::unordered_set<Ent> entities) {
    usesStorage->writeS(num, entities);
}

void PKB::setUsesP(ProcName name, std::unordered_set<Ent> entities) {
    usesStorage->writeP(name, entities);
}

void PKB::setModifiesS(StmtNum num, std::unordered_set<Ent> entities) {
    modifiesStorage->writeS(num, entities);
}

void PKB::setModifiesP(ProcName name, std::unordered_set<Ent> entities) {
    modifiesStorage->writeP(name, entities);
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
    if (followsParentMap.find(type) != followsParentMap.end()) {
        FollowsParentHandler handler(followsParentMap.at(type), statementStorage);
        return handler.handle(param1, param2);
    }
    else if (modifiesUsesMap.find(type) != modifiesUsesMap.end()) {
        ModifiesUsesHandler handler(modifiesUsesMap.at(type), statementStorage);
        return handler.handle(param1, param2);
    }
    else if (callsMap.find(type) != callsMap.end()) {
        CallsHandler handler(callsMap.at(type));
        return handler.handle(param1, param2);
    }
    else if (nextMap.find(type) != nextMap.end()) {
        if (type == RelationshipType::NEXT) {
            NextHandler handler(cfgStorage, statementStorage, procedureStorage);
            return handler.handle(param1, param2);
        }
        NextTHandler handler(cfgStorage, statementStorage, procedureStorage);
        return handler.handle(param1, param2);
    }
    return std::vector<std::vector<std::string>>();
}

std::vector<std::string> PKB::findDesignEntities(Parameter p) {
    std::vector<std::string> res;
    std::string typeString = p.getTypeString();
    if (p.getType() == ParameterType::PROCEDURE) {
        std::unordered_set<ProcName> procs = procedureStorage->getProcNames();
        for (auto proc : procs) {
            res.push_back(proc);
        }
    }
    else if (p.getType() == ParameterType::CONSTANT) {
        std::unordered_set<Const> constants = constantStorage->getConstNames();
        for (auto constant : constants) {
            res.push_back(to_string(constant));
        }
    }
    else if (p.getType() == ParameterType::VARIABLE) {
        std::unordered_set<Ent> vars = entityStorage->getEntNames();
        for (auto var : vars) {
            res.push_back(var);
        }
    }
    else if (p.isStatementRef(p)) {
        std::unordered_set<StmtNum> stmtNums = statementStorage->getStatementNumbers(typeString);
        for (auto stmtNum : stmtNums) {
            res.push_back(to_string(stmtNum));
        }
    }

    return res;
}

std::vector<std::vector<std::string>> PKB::findPattern(Pattern p) {
    // TODO: This violates LoD. Needs QPS to have a getPatternType() function
    ParameterType type = p.getPatternSyn()->getType();

    if (type == ParameterType::ASSIGN) {
        AssignPatternHandler handler(assignPatternStorage);
        return handler.handle(p);
    }
    else if (ifWhilePatternMap.find(type) != ifWhilePatternMap.end()) {
        IfWhilePatternHandler handler(ifWhilePatternMap.at(type));

        return handler.handle(p);
    }

    return std::vector<std::vector<std::string>>();
}

bool PKB::checkStatement(Stmt stmt, StmtNum num) {
    return statementStorage->checkStatement(stmt, num);
}

std::unordered_set<StmtNum> PKB::getProcedureStatementNumbers(ProcName p) {
    return procedureStorage->getProcedureStatementNumbers(p);
}

std::vector<std::pair<StmtNum, ProcName>> PKB::getCallStatements() {
    return callStorage->getCallStatements();
}

std::unordered_set<ProcName> PKB::getAllProcedureNames() {
    return procedureStorage->getProcNames();
}

std::unordered_set<Ent> PKB::getUsesS(StmtNum num) {
    return usesStorage->getEnt(num);
}

std::unordered_set<Ent> PKB::getUsesP(ProcName name) {
    return usesStorage->getEnt(name);
}

std::unordered_set<Ent> PKB::getModifiesS(StmtNum num) {
    return modifiesStorage->getEnt(num);
}

std::unordered_set<Ent> PKB::getModifiesP(ProcName name) {
    return modifiesStorage->getEnt(name);
}

std::unordered_set<StmtNum> PKB::getIfStatementNumbers() {
    return statementStorage->getStatementNumbers(IF);
}

std::unordered_set<StmtNum> PKB::getWhileStatementNumbers() {
    return statementStorage->getStatementNumbers(WHILE);
}

std::unordered_set<StmtNum> PKB::getContainedStatements(StmtNum containerNum) {
    return parentTStorage->getRightWildcard(containerNum);
}

std::pair<StmtNum, ProcName> PKB::getCallStmt(StmtNum s) {
    return callStorage->getCallStmt(s);
}

std::unordered_set<ProcName> PKB::getCallsT(ProcName p) {
    return callsTStorage->getCallees(p);
}

std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> PKB::getCFG(ProcName name) {
    return cfgStorage->getGraph(name);
}
