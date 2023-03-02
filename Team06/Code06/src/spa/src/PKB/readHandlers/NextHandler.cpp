#include "NextHandler.h"

NextHandler::NextHandler(std::shared_ptr<CFGStorage> cfgStorage, std::shared_ptr<StmtStorage> stmtStorage) {
    this->cfgStorage = cfgStorage;
    this->stmtStorage = stmtStorage;
}

std::vector<std::vector<std::string>> NextHandler::handle(Parameter param1, Parameter param2) {

    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    ParameterType paramType1 = param1.getType();
    ParameterType paramType2 = param2.getType();

    if (paramType1 == ParameterType::FIXED_INT && paramType2 == ParameterType::FIXED_INT) {
        return handleIntInt(param1, param2);
    } else if (paramType1 == ParameterType::FIXED_INT && paramType2 == ParameterType::WILDCARD) {
        return handleIntWildcard(param1);
    } else if (paramType1 == ParameterType::WILDCARD && paramType2 == ParameterType::FIXED_INT) {
        return handleWildcardInt(param2);
    } else if ((stmtTypesSet.find(paramType1) != stmtTypesSet.end()) && paramType2 == ParameterType::FIXED_INT) {
        return handleStmttypeInt(param1, param2);
    } else if (paramType1 == ParameterType::FIXED_INT && (stmtTypesSet.find(paramType2) != stmtTypesSet.end())) {
        return handleIntStmttype(param1, param2);
    } else if ((stmtTypesSet.find(paramType1) != stmtTypesSet.end()) && paramType2 == ParameterType::WILDCARD) {
        return handleStmttypeWildcard(param1);
    } else if (paramType1 == ParameterType::WILDCARD && (stmtTypesSet.find(paramType2) != stmtTypesSet.end())) {
        return handleWildcardStmttype(param2);
    } else if ((stmtTypesSet.find(paramType1) != stmtTypesSet.end()) && (stmtTypesSet.find(paramType2) != stmtTypesSet.end())) {
        return handleStmttypeStmttype(param1, param2);
    } else if (paramType1 == ParameterType::WILDCARD && paramType2 == ParameterType::WILDCARD) {
        return handleWildcardWildcard();
    }
    return std::vector<std::vector<std::string>>();
}

