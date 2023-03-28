#include "CallsHandler.h"

CallsHandler::CallsHandler(std::shared_ptr<RelationshipStorage<Ent, Ent>> callsStorage) {
    this->callsStorage = callsStorage;
}

std::vector<std::vector<std::string>> CallsHandler::handleProcnameProcname(Parameter param1, Parameter param2) {
    std::string caller = param1.getValue();
    std::string callee = param2.getValue();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<Ent> callees = callsStorage->getRightItems(caller);

    if (callees.find(callee) != callees.end()) {
        res.push_back({caller, callee});
    }
    return res;
}

std::vector<std::vector<std::string>> CallsHandler::handleProcnameWildcard(Parameter param1) {
    std::string caller = param1.getValue();
    std::vector<std::vector<std::string>> res;

    for (ProcName i : callsStorage->getRightItems(caller)) {
        res.push_back({caller, i});
    }
    return res;
}

std::vector<std::vector<std::string>> CallsHandler::handleWildcardProcname(Parameter param2) {
    std::string callee = param2.getValue();
    std::vector<std::vector<std::string>> res;
    for (ProcName caller : callsStorage->getLeftItems(callee)) {
        res.push_back({caller, callee});
    }
    return res;
}

std::vector<std::vector<std::string>> CallsHandler::handleWildcardWildcard() {
    std::vector<std::vector<std::string>> res;
    for (ProcName caller : callsStorage->getAllLeftItems()) {
        for (ProcName callee : callsStorage->getRightItems(caller)) {
            res.push_back({caller, callee});
        }
    }
    return res;
}

std::vector<std::vector<std::string>> CallsHandler::handle(Parameter param1, Parameter param2) {
    bool isProcnameParam1 = param1.isFixedStringType();
    bool isProcnameParam2 = param2.isFixedStringType();
    bool isWildcardParam1 = param1.isProcedureOnly() || param1.isWildcard();
    bool isWildcardParam2 = param2.isProcedureOnly() || param2.isWildcard();

    // based on the fact that there are no cycles in the source code
    if (param1 == param2 && !param1.isWildcard() && !param2.isWildcard()) {
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
