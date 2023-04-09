#include "FollowsParentHandler.h"

FollowsParentHandler::FollowsParentHandler(std::shared_ptr<StmtStorage>& stmtStorage) {
    this->stmtStorage = stmtStorage;
}

void FollowsParentHandler::setStorage(std::shared_ptr<RelationshipStorage<StmtNum, StmtNum>>& rlstorage) {
    this->rlStorage = rlstorage;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleIntInt(Parameter& param1, Parameter& param2) {
    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    std::vector<std::vector<std::string>> res;

    if (rlStorage->exists(stoi(paramString1), stoi(paramString2))) {
        res.push_back({paramString1, paramString2});
    }
    return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleOneIntOneSyn(Parameter& param1, Parameter& param2,
                                                                               bool isIntSyn) {

    std::string paramString = isIntSyn ? param1.getValue() : param2.getValue();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum>& stmtNums =
        isIntSyn ? rlStorage->getRightItems(stoi(paramString)) : rlStorage->getLeftItems(stoi(paramString));

    std::unordered_set<StmtNum>& typedStmtNums = isIntSyn ? stmtStorage->getStatementNumbers(param2.getTypeString())
                                                          : stmtStorage->getStatementNumbers(param1.getTypeString());

    for (StmtNum stmtNum : stmtNums) {
        if (typedStmtNums.find(stmtNum) != typedStmtNums.end()) {
            std::string stmtNumString = std::to_string(stmtNum);

            isIntSyn ? res.push_back({paramString, stmtNumString}) : res.push_back({stmtNumString, paramString});
        }
    }
    return res;
}

std::vector<std::vector<std::string>>
FollowsParentHandler::handleOneIntOneWildcard(Parameter& param1, Parameter& param2, bool isIntWildcard) {

    std::string paramString = isIntWildcard ? param1.getValue() : param2.getValue();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum>& stmtNums =
        isIntWildcard ? rlStorage->getRightItems(stoi(paramString)) : rlStorage->getLeftItems(stoi(paramString));

    if (!stmtNums.empty()) {
        return AppConstants::EARLY_RETURN_RES;
    }
    return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleSynSyn(Parameter& param1, Parameter& param2) {
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
                std::string stmtNumString1 = std::to_string(stmtNum);
                std::string stmtNumString2 = std::to_string(follower);
                res.push_back({stmtNumString1, stmtNumString2});
            }
        }
    }
    return res;
}

std::vector<std::vector<std::string>>
FollowsParentHandler::handleOneSynOneWildcard(Parameter& param1, Parameter& param2, bool isSynWildcard) {
    std::vector<std::vector<std::string>> res;

    for (auto typedStmtNum : isSynWildcard ? stmtStorage->getStatementNumbers(param1.getTypeString())
                                           : stmtStorage->getStatementNumbers(param2.getTypeString())) {

        for (StmtNum other :
             isSynWildcard ? rlStorage->getRightItems(typedStmtNum) : rlStorage->getLeftItems(typedStmtNum)) {

            std::string currString = isSynWildcard ? std::to_string(typedStmtNum) : std::to_string(other);
            std::string otherString = isSynWildcard ? std::to_string(other) : std::to_string(typedStmtNum);
            res.push_back({currString, otherString});
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

std::vector<std::vector<std::string>> FollowsParentHandler::handle(Parameter& param1, Parameter& param2) {
    bool isIntParam1 = param1.isFixedInt();
    bool isIntParam2 = param2.isFixedInt();
    bool isSynonymParam1 = !param1.isFixedInt() && !param1.isWildcard();
    bool isSynonymParam2 = !param2.isFixedInt() && !param2.isWildcard();

    if (isIntParam1) {
        if (isIntParam2) {
            return handleIntInt(param1, param2);
        }
        else if (isSynonymParam2) {
            return handleOneIntOneSyn(param1, param2, true);
        }
        else {
            return handleOneIntOneWildcard(param1, param2, true);
        }
    }
    else if (isSynonymParam1) {

        if (isIntParam2) {
            return handleOneIntOneSyn(param1, param2, false);
        }
        else if (isSynonymParam2) {
            return handleSynSyn(param1, param2);
        }
        else {
            return handleOneSynOneWildcard(param1, param2, true);
        }
    }
    else {
        if (isIntParam2) {
            return handleOneIntOneWildcard(param1, param2, false);
        }
        else if (isSynonymParam2) {
            return handleOneSynOneWildcard(param1, param2, false);
        }
        else {
            return handleWildcardWildcard();
        }
    }
    return {};
}