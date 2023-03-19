#include "CallsHandler.h"

CallsHandler::CallsHandler(std::shared_ptr<RelationshipStorage<Ent, Ent>> callsStorage) {
    this->callsStorage = callsStorage;
}

std::vector<std::vector<std::string>> CallsHandler::handleProcnameProcname(Parameter param1, Parameter param2) {
    std::string caller = param1.getValue();
    std::string callee = param2.getValue();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<ProcName> callees = callsStorage->getRightItems(caller);

    if (callees.find(callee) != callees.end()) {
        res.push_back({caller, callee});
    }
    return res;
}

std::vector<std::vector<std::string>> CallsHandler::handleProcnameWildcard(Parameter param1) {
    std::string caller = param1.getValue();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<ProcName> callees = callsStorage->getRightItems(caller);
    for (auto i : callees) {
        res.push_back({caller, i});
    }
    return res;
}

std::vector<std::vector<std::string>> CallsHandler::handleWildcardProcname(Parameter param2) {
    std::string callee = param2.getValue();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<ProcName> callers = callsStorage->getLeftItems(callee);
    for (auto i : callers) {
        res.push_back({i, callee});
    }
    return res;
}

std::vector<std::vector<std::string>> CallsHandler::handleWildcardWildcard() {
    std::vector<std::vector<std::string>> res;
    for (auto caller : callsStorage->getAllLeftItems()) {
        for (auto callee : callsStorage->getRightItems(caller)) {
            res.push_back({caller, callee});
        }
    }
    return res;
}

std::vector<std::vector<std::string>> CallsHandler::handle(Parameter param1, Parameter param2) {
    ParameterType paramType1 = param1.getType();
    ParameterType paramType2 = param2.getType();
    bool isProcnameParam1 = paramType1 == ParameterType::FIXED_STRING;
    bool isProcnameParam2 = paramType2 == ParameterType::FIXED_STRING;
    bool isWildcardParam1 = paramType1 == ParameterType::PROCEDURE || paramType1 == ParameterType::WILDCARD;
    bool isWildcardParam2 = paramType2 == ParameterType::PROCEDURE || paramType2 == ParameterType::WILDCARD;

    // based on the fact that there are no cycles in the source code
    if (param1 == param2 && !(paramType1 == ParameterType::WILDCARD) && !(paramType2 == ParameterType::WILDCARD)) {
        return {};
    }

    if (isProcnameParam1) {
        if (isProcnameParam2) {
            return handleProcnameProcname(param1, param2);
        }
        return handleProcnameWildcard(param1);
    }
    else if (isProcnameParam2) {
        return handleWildcardProcname(param2);
    }
    else if (isWildcardParam1 && isWildcardParam2) {
        return handleWildcardWildcard();
    }
    return std::vector<std::vector<std::string>>();
}
