#include "CallsHandler.h"

CallsHandler::CallsHandler(std::shared_ptr<CallsStorage> callsStorage, bool isTransitive) {
    this->callsStorage = callsStorage;
    this->isTransitive = isTransitive;
}

std::vector<std::vector<std::string>> CallsHandler::handleProcnameProcname(Parameter param1, Parameter param2) {

}

std::vector<std::vector<std::string>> CallsHandler::handleProcnameWildcard(Parameter param1, Parameter param2) {
}

std::vector<std::vector<std::string>> CallsHandler::handleWildcardProcname(Parameter param1, Parameter param2) {
}

std::vector<std::vector<std::string>> CallsHandler::handleWildcardWildcard() {
}

std::vector<std::vector<std::string>> CallsHandler::handleProcnameProcnameTransitive(Parameter param1, Parameter param2) {
}

std::vector<std::vector<std::string>> CallsHandler::handleProcnameWildcardTransitive(Parameter param1, Parameter param2) {
}

std::vector<std::vector<std::string>> CallsHandler::handleWildcardProcnameTransitive(Parameter param1, Parameter param2) {
}

std::vector<std::vector<std::string>> CallsHandler::handleWildcardWildcardTransitive() {
}

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
            return isTransitive ? handleProcnameProcnameTransitive(param1, param2) : handleProcnameProcname(param1, param2);
        }
        return isTransitive ? handleProcnameWildcardTransitive(param1, param2) : handleProcnameWildcard(param1, param2);
    } else if (isProcnameParam2) {
        return isTransitive ? handleWildcardProcnameTransitive(param1, param2) : handleWildcardProcname(param1, param2);
    } else if (isWildcardParam1 && isWildcardParam2) {
        return isTransitive ? handleWildcardWildcardTransitive() : handleWildcardWildcard();
    }
    return std::vector<std::vector<std::string>>();
}



