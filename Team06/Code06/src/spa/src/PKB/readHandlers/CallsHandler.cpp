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

std::vector<std::vector<std::string>> CallsHandler::handleWildcardWildcard(Parameter param1, Parameter param2) {
}

std::vector<std::vector<std::string>> CallsHandler::handle(Parameter param1, Parameter param2) {
}

