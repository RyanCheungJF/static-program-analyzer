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

    std::unordered_set<StmtNum> stmtNums = *rlStorage->getRightItems(stoi(paramString1));
    for (StmtNum stmtNum : stmtNums) {
        if (stmtStorage->getStatementNumbers(param2.getTypeString()).find(stmtNum) !=
                stmtStorage->getStatementNumbers(param2.getTypeString()).end()) {
            std::string stmtNumString = to_string(stmtNum);
            res.push_back({paramString1, stmtNumString});
        }
    }
    return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleIntWildcard(Parameter param1) {
    std::string paramString1 = param1.getValue();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum> stmtNums = *rlStorage->getRightItems(stoi(paramString1));
    for (StmtNum stmtNum : stmtNums) {
        std::string stmtNumString = to_string(stmtNum);
        res.push_back({paramString1, stmtNumString});
    }
    return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleSynInt(Parameter param1, Parameter param2) {
    std::string paramString2 = param2.getValue();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum> stmtNums = *rlStorage->getLeftItems(stoi(paramString2));
    for (StmtNum stmtNum : stmtNums) {
        if (stmtStorage->getStatementNumbers(param1.getTypeString()).find(stmtNum) !=
                stmtStorage->getStatementNumbers(param1.getTypeString()).end()) {
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

    for (auto stmtNum : stmtStorage->getStatementNumbers(param1.getTypeString())) {
        std::unordered_set<StmtNum> followers = *rlStorage->getRightItems(stmtNum);
        for (StmtNum follower : followers) {
            if (stmtStorage->getStatementNumbers(param2.getTypeString()).find(follower) !=
                    stmtStorage->getStatementNumbers(param2.getTypeString()).end()) {
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
        std::unordered_set<StmtNum> followers = *rlStorage->getRightItems(typedStmtNum);
        for (StmtNum follower : followers) {
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

    std::unordered_set<StmtNum> stmtNums = *rlStorage->getLeftItems(stoi(paramString2));
    for (StmtNum stmtNum : stmtNums) {
        std::string stmtNumString = to_string(stmtNum);
        res.push_back({stmtNumString, paramString2});
    }
    return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleWildcardSyn(Parameter param2) {
    std::vector<std::vector<std::string>> res;

    for (auto typedStmtNum : stmtStorage->getStatementNumbers(param2.getTypeString())) {
        std::unordered_set<StmtNum> followees = *rlStorage->getLeftItems(typedStmtNum);
        for (StmtNum followee : followees) {
            std::string stmtNumString1 = to_string(followee);
            std::string stmtNumString2 = to_string(typedStmtNum);
            res.push_back({stmtNumString1, stmtNumString2});
        }
    }
    return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleWildcardWildcard() {
    std::vector<std::vector<std::string>> res;
    for (StmtNum followee : *rlStorage->getAllLeftItems()) {
        for (StmtNum follower : *rlStorage->getRightItems(followee)) {
            res.push_back({std::to_string(followee), std::to_string(follower)});
        }
    }
    return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handle(Parameter param1, Parameter param2) {
    ParameterType paramType1 = param1.getType();
    ParameterType paramType2 = param2.getType();
    bool isIntParam1 = paramType1 == ParameterType::FIXED_INT;
    bool isIntParam2 = paramType2 == ParameterType::FIXED_INT;
    bool isSynonymParam1 = paramType1 != ParameterType::FIXED_INT && paramType1 != ParameterType::WILDCARD;
    bool isSynonymParam2 = paramType2 != ParameterType::FIXED_INT && paramType2 != ParameterType::WILDCARD;

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