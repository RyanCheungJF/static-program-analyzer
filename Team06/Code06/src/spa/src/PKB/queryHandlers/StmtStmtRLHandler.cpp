#include "StmtStmtRLHandler.h"

std::vector<std::vector<std::string>> StmtStmtRLHandler::handle(StmtStmtRLStorage *storage, std::string param1, std::string param2) {
	std::vector<std::vector<std::string>> res;
	bool isIntParam1 = Parameter::guessParameterType(param1) == ParameterType::FIXED_INT;
	bool isIntParam2 = Parameter::guessParameterType(param2) == ParameterType::FIXED_INT;
	if (isIntParam1) {
		// e.g. FollowsT(1, 2)
		if (Parameter::guessParameterType(param2) == ParameterType::FIXED_INT) {
			if (storage->exists(stoi(param1), stoi(param2))) {
				res.push_back({ param1, param2 });
			}
			return res;
		}
		// e.g. FollowsT(1, _)
		else {
			std::vector<StmtNum> stmtNums = storage->getRightWildcard(stoi(param1));
			for (auto stmtNum : stmtNums) {
				std::string stmtNumString = to_string(stmtNum);
				res.push_back({ param1, stmtNumString });
			}
			return res;
		}
	}
	else {
		// e.g. FollowsT(_, 2)
		if (isIntParam2) {
			std::vector<StmtNum> stmtNums = storage->getLeftWildcard(stoi(param2));
			for (auto stmtNum : stmtNums) {
				std::string stmtNumString = to_string(stmtNum);
				res.push_back({ stmtNumString, param2 });
			}
			return res;
		}
		// e.g. FollowsT(_, _)
		else {
			std::pair<std::vector<StmtNum>, std::vector<StmtNum>> stmtNumsArrayPair = storage->getAllPairs();
			std::vector<StmtNum> leftStmtNums = stmtNumsArrayPair.first;
			std::vector<StmtNum> rightStmtNums = stmtNumsArrayPair.second;
			for (int i = 0; i < leftStmtNums.size(); i++) {
				std::string leftNumString = to_string(leftStmtNums[i]);
				std::string rightNumString = to_string(rightStmtNums[i]);
				res.push_back({ leftNumString, rightNumString });
			}
			return res;
		}
	}
}