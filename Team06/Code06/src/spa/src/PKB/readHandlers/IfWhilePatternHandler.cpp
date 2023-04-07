#include "IfWhilePatternHandler.h"

void IfWhilePatternHandler::setStorage(std::shared_ptr<PatternStorage>& patternStorage) {
    this->patternStorage = patternStorage;
}

std::vector<std::vector<std::string>> IfWhilePatternHandler::handleVar(Ent ent) {
    std::vector<std::vector<std::string>> res;
    for (auto stmtNum : patternStorage->getStmtNums(ent)) {
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

std::vector<std::vector<std::string>> IfWhilePatternHandler::handle(Pattern& p) {
    Parameter lhs = p.getEntRef();
    std::string lhsString = p.getEntRefValue();

    if (lhs.isFixedStringType()) {
        return handleVar(lhsString);
    }
    else {
        return handleWildcard();
    }
}