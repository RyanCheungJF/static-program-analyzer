#include "StmtStmtRLHandler.h"

std::vector<std::vector<std::string>> StmtStmtRLHandler::handle(StmtStmtRLStorage* rlStorage, StmtStorage* stmtStorage, Parameter param1, Parameter param2) {
	std::vector<std::vector<std::string>> res;
	std::string paramString1 = param1.getValue();
	std::string paramString2 = param2.getValue();
	bool isIntParam1 = Parameter::guessParameterType(paramString1) == ParameterType::FIXED_INT;
	bool isIntParam2 = Parameter::guessParameterType(paramString2) == ParameterType::FIXED_INT;
	bool isSynonymParam1 = Parameter::guessParameterType(paramString1) == ParameterType::SYNONYM;
	bool isSynonymParam2 = Parameter::guessParameterType(paramString2) == ParameterType::SYNONYM;
	if (isIntParam1) {
		// e.g. FollowsT(1, 2)
		if (isIntParam2) {
			if (rlStorage->exists(stoi(paramString1), stoi(paramString2))) {
				res.push_back({ paramString1, paramString2 });
			}
		}

		// e.g. FollowsT(1, if)
		else if (isSynonymParam2) {
			std::vector<StmtNum> stmtNums = rlStorage->getRightWildcard(stoi(paramString1));
			std::unordered_set<StmtNum> typedStmtNums = stmtStorage->getStatementNumbers(param2.getTypeString());
			for (auto stmtNum : stmtNums) {
				if (typedStmtNums.find(stmtNum) != typedStmtNums.end()) {
					std::string stmtNumString = to_string(stmtNum);
					res.push_back({ paramString1, stmtNumString });
				}
			}
			
		}

		// e.g. FollowsT(1, _)
		else {
			std::vector<StmtNum> stmtNums = rlStorage->getRightWildcard(stoi(paramString1));
			for (auto stmtNum : stmtNums) {
				std::string stmtNumString = to_string(stmtNum);
				res.push_back({ paramString1, stmtNumString });
			}
		}
	}
	else if (isSynonymParam1) {
		// e.g. FollowsT(a, 2)
		if (isIntParam2) {
			std::vector<StmtNum> stmtNums = rlStorage->getLeftWildcard(stoi(paramString2));
			std::unordered_set<StmtNum> typedStmtNums = stmtStorage->getStatementNumbers(param1.getTypeString());
			for (auto stmtNum : stmtNums) {
				if (typedStmtNums.find(stmtNum) != typedStmtNums.end()) {
					std::string stmtNumString = to_string(stmtNum);
					res.push_back({ stmtNumString, paramString2 });
				}
			}
		}
		// e.g. FollowsT(a, while)
		else if (isSynonymParam2) {
			if (paramString1 == paramString2) return res;
			std::unordered_set<StmtNum> typedStmtNums1 = stmtStorage->getStatementNumbers(param1.getTypeString());
			std::unordered_set<StmtNum> typedStmtNums2 = stmtStorage->getStatementNumbers(param2.getTypeString());
			for (auto stmtNum : typedStmtNums1) {
				std::vector<StmtNum> followers = rlStorage->getRightWildcard(stmtNum);
				for (auto follower : followers) {
					if (typedStmtNums2.find(follower) != typedStmtNums2.end()) {
						std::string stmtNumString1 = to_string(stmtNum);
						std::string stmtNumString2 = to_string(follower);
						res.push_back({ stmtNumString1, stmtNumString2 });
					}
				}
			}
		}
		// e.g. FollowsT(a, _)
		else {
			std::unordered_set<StmtNum> typedStmtNums = stmtStorage->getStatementNumbers(param1.getTypeString());
			for (auto typedStmtNum : typedStmtNums) {
				std::vector<StmtNum> followers = rlStorage->getRightWildcard(typedStmtNum);
				for (auto follower : followers) {
					std::string stmtNumString1 = to_string(typedStmtNum);
					std::string stmtNumString2 = to_string(follower);
					res.push_back({ stmtNumString1, stmtNumString2 });
				}
			}
		}
	} else {
		// e.g. FollowsT(_, 2)
		if (isIntParam2) {
			std::vector<StmtNum> stmtNums = rlStorage->getLeftWildcard(stoi(paramString2));
			for (auto stmtNum : stmtNums) {
				std::string stmtNumString = to_string(stmtNum);
				res.push_back({ stmtNumString, paramString2 });
			}
		}
		// .e.g FollowsT(_, a)
		else if (isSynonymParam2) {
			std::unordered_set<StmtNum> typedStmtNums = stmtStorage->getStatementNumbers(param2.getTypeString());
			for (auto typedStmtNum : typedStmtNums) {
				std::vector<StmtNum> followees = rlStorage->getLeftWildcard(typedStmtNum);
				for (auto followee : followees) {
					std::string stmtNumString1 = to_string(typedStmtNum);
					std::string stmtNumString2 = to_string(followee);
					res.push_back({ stmtNumString1, stmtNumString2 });
				}
			}
		}
		// e.g. FollowsT(_, _)
		else {
			std::pair<std::vector<StmtNum>, std::vector<StmtNum>> stmtNumsArrayPair = rlStorage->getAllPairs();
			std::vector<StmtNum> leftStmtNums = stmtNumsArrayPair.first;
			std::vector<StmtNum> rightStmtNums = stmtNumsArrayPair.second;
			for (int i = 0; i < leftStmtNums.size(); i++) {
				std::string leftNumString = to_string(leftStmtNums[i]);
				std::string rightNumString = to_string(rightStmtNums[i]);
				res.push_back({ leftNumString, rightNumString });
			}
		}
	}
	return res;
}