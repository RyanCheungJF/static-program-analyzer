#include "FollowsParentHandler.h"

FollowsParentHandler::FollowsParentHandler(std::shared_ptr<RelationshipStorage<StmtNum, StmtNum>> rlStorage,
                                           std::shared_ptr<StmtStorage> stmtStorage) {
    this->rlStorage = rlStorage;
    this->stmtStorage = stmtStorage;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleIntInt(Parameter param1, Parameter param2) {
    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    std::vector<std::vector<std::string>> res;

    if (rlStorage->exists(stoi(paramString1), stoi(paramString2))) {
        res.push_back({paramString1, paramString2});
    }
    return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleIntSyn(Parameter param1, Parameter param2) {
    std::string paramString1 = param1.getValue();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum>& stmtNums = rlStorage->getRightItems(stoi(paramString1));
    std::unordered_set<StmtNum>& typedStmtNums = stmtStorage->getStatementNumbers(param2.getTypeString());

    for (StmtNum stmtNum : stmtNums) {
        if (typedStmtNums.find(stmtNum) != typedStmtNums.end()) {
            std::string stmtNumString = to_string(stmtNum);
            res.push_back({paramString1, stmtNumString});
        }
    }
    return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleIntWildcard(Parameter param1) {
    std::string paramString1 = param1.getValue();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum>& stmtNums = rlStorage->getRightItems(stoi(paramString1));

    if (!stmtNums.empty()) {
        return AppConstants::EARLY_RETURN_RES;
    }
    return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleSynInt(Parameter param1, Parameter param2) {
    std::string paramString2 = param2.getValue();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum>& stmtNums = rlStorage->getLeftItems(stoi(paramString2));
    std::unordered_set<StmtNum>& typedStmtNums = stmtStorage->getStatementNumbers(param1.getTypeString());

    for (StmtNum stmtNum : stmtNums) {
        if (typedStmtNums.find(stmtNum) != typedStmtNums.end()) {
            std::string stmtNumString = to_string(stmtNum);
            res.push_back({stmtNumString, paramString2});
        }
    }
    return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleSynSyn(Parameter param1, Parameter param2) {
    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    std::vector<std::vector<std::string>> res;

    if (paramString1 == paramString2)
        return res;

    std::unordered_set<StmtNum>& typedStmtNums1 = stmtStorage->getStatementNumbers(param1.getTypeString());
    std::unordered_set<StmtNum>& typedStmtNums2 = stmtStorage->getStatementNumbers(param2.getTypeString());

    for (auto stmtNum : typedStmtNums1) {
        for (StmtNum follower : rlStorage->getRightItems(stmtNum)) {
            if (typedStmtNums2.find(follower) != typedStmtNums2.end()) {
                std::string stmtNumString1 = to_string(stmtNum);
                std::string stmtNumString2 = to_string(follower);
                res.push_back({stmtNumString1, stmtNumString2});
            }
        }
    }
    return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleSynWildcard(Parameter param1) {
    std::vector<std::vector<std::string>> res;

    for (auto typedStmtNum : stmtStorage->getStatementNumbers(param1.getTypeString())) {
        for (StmtNum follower : rlStorage->getRightItems(typedStmtNum)) {
            std::string stmtNumString1 = to_string(typedStmtNum);
            std::string stmtNumString2 = to_string(follower);
            res.push_back({stmtNumString1, stmtNumString2});
        }
    }
    return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleWildcardInt(Parameter param2) {
    std::string paramString2 = param2.getValue();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum>& stmtNums = rlStorage->getLeftItems(stoi(paramString2));

    if (!stmtNums.empty()) {
        return AppConstants::EARLY_RETURN_RES;
    }
    return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleWildcardSyn(Parameter param2) {
    std::vector<std::vector<std::string>> res;

    for (auto typedStmtNum : stmtStorage->getStatementNumbers(param2.getTypeString())) {
        for (StmtNum followee : rlStorage->getLeftItems(typedStmtNum)) {
            std::string stmtNumString1 = to_string(followee);
            std::string stmtNumString2 = to_string(typedStmtNum);
            res.push_back({stmtNumString1, stmtNumString2});
        }
    }
    return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleWildcardWildcard() {
    std::vector<std::vector<std::string>> res;

    for (StmtNum followee : rlStorage->getAllLeftItems()) {
        if (!rlStorage->getRightItems(followee).empty()) {
            return AppConstants::EARLY_RETURN_RES;
        }
    }
    return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handle(Parameter param1, Parameter param2) {
    bool isIntParam1 = param1.isFixedInt();
    bool isIntParam2 = param2.isFixedInt();
    bool isSynonymParam1 = !param1.isFixedInt() && !param1.isWildcard();
    bool isSynonymParam2 = !param2.isFixedInt() && !param2.isWildcard();

    if (isIntParam1) {
        if (isIntParam2) {
            return handleIntInt(param1, param2);
        }
        else if (isSynonymParam2) {
            return handleIntSyn(param1, param2);
        }
        else {
            return handleIntWildcard(param1);
        }
    }
    else if (isSynonymParam1) {

        if (isIntParam2) {
            return handleSynInt(param1, param2);
        }
        else if (isSynonymParam2) {
            return handleSynSyn(param1, param2);
        }
        else {
            return handleSynWildcard(param1);
        }
    }
    else {
        if (isIntParam2) {
            return handleWildcardInt(param2);
        }
        else if (isSynonymParam2) {
            return handleWildcardSyn(param2);
        }
        else {
            return handleWildcardWildcard();
        }
    }
    return std::vector<std::vector<std::string>>();
}