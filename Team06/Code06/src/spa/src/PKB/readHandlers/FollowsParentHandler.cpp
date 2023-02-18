#include "FollowsParentHandler.h"

FollowsParentHandler::FollowsParentHandler(std::shared_ptr<FollowsParentStorage> rlStorage, std::shared_ptr<StmtStorage> stmtStorage) {
	this->rlStorage = rlStorage;
	this->stmtStorage = stmtStorage;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleIntInt(Parameter param1, Parameter param2) {
	std::string paramString1 = param1.getValue();
	std::string paramString2 = param2.getValue();
	std::vector<std::vector<std::string>> res;

	if (rlStorage->exists(stoi(paramString1), stoi(paramString2))) {
		res.push_back({ paramString1, paramString2 });
	}
	return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleIntSyn(Parameter param1, Parameter param2) {
	std::string paramString1 = param1.getValue();
	std::vector<std::vector<std::string>> res;

	std::unordered_set<StmtNum> stmtNums = rlStorage->getRightWildcard(stoi(paramString1));
	std::unordered_set<StmtNum> typedStmtNums = stmtStorage->getStatementNumbers(param2.getTypeString());
	for (auto stmtNum : stmtNums) {
		if (typedStmtNums.find(stmtNum) != typedStmtNums.end()) {
			std::string stmtNumString = to_string(stmtNum);
			res.push_back({ paramString1, stmtNumString });
		}
	}
	return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleIntWildcard(Parameter param1, Parameter param2) {
	std::string paramString1 = param1.getValue();
	std::vector<std::vector<std::string>> res;

	std::unordered_set<StmtNum> stmtNums = rlStorage->getRightWildcard(stoi(paramString1));
	for (auto stmtNum : stmtNums) {
		std::string stmtNumString = to_string(stmtNum);
		res.push_back({ paramString1, stmtNumString });
	}
	return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleSynInt(Parameter param1, Parameter param2) {
	std::string paramString2 = param2.getValue();
	std::vector<std::vector<std::string>> res;

	std::unordered_set<StmtNum> stmtNums = rlStorage->getLeftWildcard(stoi(paramString2));
	std::unordered_set<StmtNum> typedStmtNums = stmtStorage->getStatementNumbers(param1.getTypeString());
	for (auto stmtNum : stmtNums) {
		if (typedStmtNums.find(stmtNum) != typedStmtNums.end()) {
			std::string stmtNumString = to_string(stmtNum);
			res.push_back({ stmtNumString, paramString2 });
		}
	}
	return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleSynSyn(Parameter param1, Parameter param2) {
	std::string paramString1 = param1.getValue();
	std::string paramString2 = param2.getValue();
	std::vector<std::vector<std::string>> res;

	if (paramString1 == paramString2) return res;

	std::unordered_set<StmtNum> typedStmtNums1 = stmtStorage->getStatementNumbers(param1.getTypeString());
	std::unordered_set<StmtNum> typedStmtNums2 = stmtStorage->getStatementNumbers(param2.getTypeString());
	for (auto stmtNum : typedStmtNums1) {
		std::unordered_set<StmtNum> followers = rlStorage->getRightWildcard(stmtNum);
		for (auto follower : followers) {
			if (typedStmtNums2.find(follower) != typedStmtNums2.end()) {
				std::string stmtNumString1 = to_string(stmtNum);
				std::string stmtNumString2 = to_string(follower);
				res.push_back({ stmtNumString1, stmtNumString2 });
			}
		}
	}
	return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleSynWildcard(Parameter param1, Parameter param2) {
	std::string paramString1 = param1.getValue();
	std::string paramString2 = param2.getValue();
	std::vector<std::vector<std::string>> res;

	std::unordered_set<StmtNum> typedStmtNums = stmtStorage->getStatementNumbers(param1.getTypeString());
	for (auto typedStmtNum : typedStmtNums) {
		std::unordered_set<StmtNum> followers = rlStorage->getRightWildcard(typedStmtNum);
		for (auto follower : followers) {
			std::string stmtNumString1 = to_string(typedStmtNum);
			std::string stmtNumString2 = to_string(follower);
			res.push_back({ stmtNumString1, stmtNumString2 });
		}
	}
	return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleWildcardInt(Parameter param1, Parameter param2) {
	std::string paramString2 = param2.getValue();
	std::vector<std::vector<std::string>> res;

	std::unordered_set<StmtNum> stmtNums = rlStorage->getLeftWildcard(stoi(paramString2));
	for (auto stmtNum : stmtNums) {
		std::string stmtNumString = to_string(stmtNum);
		res.push_back({ stmtNumString, paramString2 });
	}
	return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleWildcardSyn(Parameter param1, Parameter param2) {
	std::string paramString2 = param2.getValue();
	std::vector<std::vector<std::string>> res;
 
	std::unordered_set<StmtNum> typedStmtNums = stmtStorage->getStatementNumbers(param2.getTypeString());
	for (auto typedStmtNum : typedStmtNums) {
		std::unordered_set<StmtNum> followees = rlStorage->getLeftWildcard(typedStmtNum);
		for (auto followee : followees) {
			std::string stmtNumString1 = to_string(followee);
			std::string stmtNumString2 = to_string(typedStmtNum);
			res.push_back({ stmtNumString1, stmtNumString2 });
		}
	}
	return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handleWildcardWildcard(Parameter param1, Parameter param2) {
	std::vector<std::vector<std::string>> res;

	std::pair<std::vector<StmtNum>, std::vector<StmtNum>> stmtNumsArrayPair = rlStorage->getAllPairs();
	std::vector<StmtNum> leftStmtNums = stmtNumsArrayPair.first;
	std::vector<StmtNum> rightStmtNums = stmtNumsArrayPair.second;
	for (int i = 0; i < leftStmtNums.size(); i++) {
		std::string leftNumString = to_string(leftStmtNums[i]);
		std::string rightNumString = to_string(rightStmtNums[i]);
		res.push_back({ leftNumString, rightNumString });
	}
	return res;
}

std::vector<std::vector<std::string>> FollowsParentHandler::handle(Parameter param1, Parameter param2) {
	
	std::string paramString1 = param1.getValue();
	std::string paramString2 = param2.getValue();
	bool isIntParam1 = Parameter::guessParameterType(paramString1) == ParameterType::FIXED_INT;
	bool isIntParam2 = Parameter::guessParameterType(paramString2) == ParameterType::FIXED_INT;
	bool isSynonymParam1 = Parameter::guessParameterType(paramString1) == ParameterType::SYNONYM;
	bool isSynonymParam2 = Parameter::guessParameterType(paramString2) == ParameterType::SYNONYM;

	if (isIntParam1) {
		if (isIntParam2) {
			return handleIntInt(param1, param2);
		}
		else if (isSynonymParam2) {
			return handleIntSyn(param1, param2);
		}
		else {
			return handleIntWildcard(param1, param2);
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
			return handleSynWildcard(param1, param2);
		}
	} else {
		if (isIntParam2) {
			return handleWildcardInt(param1, param2);
		}
		else if (isSynonymParam2) {
			return handleWildcardSyn(param1, param2);
		}
		else {
			return handleWildcardWildcard(param1, param2);
		}
	}
	return std::vector<std::vector<std::string>>();
}