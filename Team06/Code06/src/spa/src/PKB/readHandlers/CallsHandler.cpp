#include "CallsHandler.h"

void CallsHandler::setStorage(std::shared_ptr<RelationshipStorage<Ent, Ent>> callsStorage) {
    this->callsStorage = callsStorage;
}

std::vector<std::vector<std::string>> CallsHandler::handleProcnameProcname(Parameter& param1, Parameter& param2) {
    std::string caller = param1.getValue();
    std::string callee = param2.getValue();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<Ent> callees = callsStorage->getRightItems(caller);

    if (callees.find(callee) != callees.end()) {
        res.push_back({caller, callee});
    }
    return res;
}

std::vector<std::vector<std::string>>
CallsHandler::handleOneProcnameOneWildcard(Parameter& param, bool isProcnameWildcard, bool isEarlyReturn) {

    std::string curr = param.getValue();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<ProcName>& others =
        isProcnameWildcard ? callsStorage->getRightItems(curr) : callsStorage->getLeftItems(curr);

    if (isEarlyReturn && !others.empty()) {
        return AppConstants::EARLY_RETURN_RES;
    }

    for (ProcName val : others) {
        isProcnameWildcard ? res.push_back({curr, val}) : res.push_back({val, curr});
    }
    return res;
}

std::vector<std::vector<std::string>> CallsHandler::handleWildcardWildcard(bool isEarlyReturn) {
    std::vector<std::vector<std::string>> res;
    for (ProcName caller : callsStorage->getAllLeftItems()) {
        if (isEarlyReturn) {
            return AppConstants::EARLY_RETURN_RES;
        }
        for (ProcName callee : callsStorage->getRightItems(caller)) {
            res.push_back({caller, callee});
        }
    }
    return res;
}

std::vector<std::vector<std::string>> CallsHandler::handle(Parameter& param1, Parameter& param2) {
    bool isProcnameParam1 = param1.isFixedStringType();
    bool isProcnameParam2 = param2.isFixedStringType();
    bool isWildcardParam1 = param1.isWildcard();
    bool isWildcardParam2 = param2.isWildcard();

    // based on the fact that there are no cycles in the source code
    if (param1 == param2 && !isWildcardParam1 && !isWildcardParam2) {
        return {};
    }

    if (isProcnameParam1) {
        if (isProcnameParam2) {
            return handleProcnameProcname(param1, param2);
        }
        return handleOneProcnameOneWildcard(param1, true, isWildcardParam2);
    }
    else if (isProcnameParam2) {
        return handleOneProcnameOneWildcard(param2, false, isWildcardParam1);
    }
    else if (isWildcardParam1 && isWildcardParam2) {
        return handleWildcardWildcard(AppConstants::IS_EARLY_RETURN);
    }
    else if (param1.isProcedureOnly() || param2.isProcedureOnly()) {
        return handleWildcardWildcard(!AppConstants::IS_EARLY_RETURN);
    }
    return std::vector<std::vector<std::string>>();
}
