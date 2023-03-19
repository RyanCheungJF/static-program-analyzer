#include "IfWhilePatternHandler.h"

IfWhilePatternHandler::IfWhilePatternHandler(std::shared_ptr<PatternStorage> patternStorage) {
    this->patternStorage = patternStorage;
}

std::vector<std::vector<std::string>> IfWhilePatternHandler::handleVar(Ent ent) {
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum> stmtNums = patternStorage->getStmtNums(ent);
    for (auto stmtNum : stmtNums) {
        res.push_back({std::to_string(stmtNum), ent});
    }

    return res;
}

std::vector<std::vector<std::string>> IfWhilePatternHandler::handleWildcard() {
    std::vector<std::vector<std::string>> res;

    std::pair<std::vector<StmtNum>, std::vector<Ent>> stmtNumVarPairs = patternStorage->getAllPairs();
    std::vector<StmtNum> stmtNums = stmtNumVarPairs.first;
    std::vector<Ent> vars = stmtNumVarPairs.second;

    for (int i = 0; i < stmtNums.size(); i++) {
        res.push_back({std::to_string(stmtNums[i]), vars[i]});
    }

    return res;
}

std::vector<std::vector<std::string>> IfWhilePatternHandler::handle(Pattern p) {
    ParameterType lhsType = p.getEntRefType();
    std::string lhsString = p.getEntRefValue();

    if (lhsType == ParameterType::FIXED_STRING) {
        return handleVar(lhsString);
    }
    else {
        return handleWildcard();
    }
}