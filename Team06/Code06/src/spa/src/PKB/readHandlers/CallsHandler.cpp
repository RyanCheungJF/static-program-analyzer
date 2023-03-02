#include "CallsHandler.h"

CallsHandler::CallsHandler(std::shared_ptr<CallStorage> callStorage, std::shared_ptr<StmtStorage> stmtStorage) {
    this->callStorage = callStorage;
    this->stmtStorage = stmtStorage;
}

std::vector<std::vector<std::string>> CallsHandler::handleProcnameProcname(Parameter param1, Parameter param2) {
}

std::vector<std::vector<std::string>> CallsHandler::handleProcnameWildcard(Parameter param1, Parameter param2) {
}

std::vector<std::vector<std::string>> CallsHandler::handleWildcardProcname(Parameter param1, Parameter param2) {
}

std::vector<std::vector<std::string>> CallsHandler::handleWildcardWildcard() {
}

// TODO: how to distinguish between transitive and non-transitive calls?
std::vector<std::vector<std::string>> CallsHandler::handle(Parameter param1, Parameter param2) {

    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    ParameterType paramType1 = param1.getType();
    ParameterType paramType2 = param2.getType();
    bool isProcnameParam1 = paramType1 == ParameterType::PROCEDURE;
    bool isProcnameParam2 = paramType2 == ParameterType::PROCEDURE;
    bool isWildcardParam1 = paramType1 != ParameterType::PROCEDURE && (
            paramType1 == ParameterType::WILDCARD || paramType1 == ParameterType::SYNONYM
            );
    bool isWildcardParam2 = paramType2 != ParameterType::PROCEDURE && (
            paramType2 == ParameterType::WILDCARD || paramType2 == ParameterType::SYNONYM
            );

    if (isProcnameParam1) {
        if (isProcnameParam2) {
            return handleProcnameProcname(param1, param2);
        }
        return handleProcnameWildcard(param1, param2);
    } else if (isProcnameParam2) {
        return handleWildcardProcname(param1, param2);
    } else if (isWildcardParam1 && isWildcardParam2) {
        return handleWildcardWildcard();
    }
    return std::vector<std::vector<std::string>>();
}

