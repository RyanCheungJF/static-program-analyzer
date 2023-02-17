#include "ModifiesUsesHandler.h"

ModifiesUsesHandler::ModifiesUsesHandler(std::shared_ptr<ModifiesUsesStorage> rlStorage, std::shared_ptr<StmtStorage> stmtStorage) {
	this->rlStorage = rlStorage;
	this->stmtStorage = stmtStorage;
}

std::vector<std::vector<std::string>> ModifiesUsesHandler::handleIntVar(Parameter param1, Parameter param2) {
	std::string paramString1 = param1.getValue();
	std::string paramString2 = param2.getValue();
	std::vector<std::vector<std::string>> res;

	if (rlStorage->exists(stoi(paramString1), paramString2)) {
		res.push_back({ paramString1, paramString2 });
	}
	return res;
}

std::vector<std::vector<std::string>> ModifiesUsesHandler::handleIntWildcard(Parameter fixedIntParam) {
	std::string fixedIntString = fixedIntParam.getValue();
	std::vector<std::vector<std::string>> res;

	std::unordered_set<Ent> entities = rlStorage->getEnt(stoi(fixedIntString));
	for (auto entity : entities) {
		res.push_back({ fixedIntString, entity });
	}
	return res;
}

std::vector<std::vector<std::string>> ModifiesUsesHandler::handleProcVar(Parameter param1, Parameter param2) {
	std::string paramString1 = param1.getValue();
	std::string paramString2 = param2.getValue();
	std::vector<std::vector<std::string>> res;

	if (rlStorage->exists(paramString1, paramString2)) {
		res.push_back({ paramString1, paramString2 });
	}
	return res;
}

std::vector<std::vector<std::string>> ModifiesUsesHandler::handleProcWildcard(Parameter fixedProcParam) {
	std::string fixedProc = fixedProcParam.getValue();
	std::vector<std::vector<std::string>> res;

	std::unordered_set<Ent> entities = rlStorage->getEnt(fixedProc);
	for (auto entity : entities) {
		res.push_back({ fixedProc, entity });
	}
	return res;
}

std::vector<std::vector<std::string>> ModifiesUsesHandler::handleStmtSynVar(Parameter param1, Parameter param2) {
	std::string paramString2 = param2.getValue();
	std::vector<std::vector<std::string>> res;

	std::unordered_set<StmtNum> stmtNums = rlStorage->getStmtsFromEnt(paramString2);
	std::unordered_set<StmtNum> typedStmtNums = stmtStorage->getStatementNumbers(param1.getTypeString());
	for (auto stmtNum : stmtNums) {
		if (typedStmtNums.find(stmtNum) != typedStmtNums.end()) {
			std::string stmtNumString = to_string(stmtNum);
			res.push_back({ stmtNumString, paramString2 });
		}
	}
	return res;
}

std::vector<std::vector<std::string>> ModifiesUsesHandler::handleStmtSynWildcard(Parameter param1, Parameter param2) {
	std::string paramString2 = param2.getValue();
	std::vector<std::vector<std::string>> res;

	std::unordered_set<StmtNum> typedStmtNums = stmtStorage->getStatementNumbers(param1.getTypeString());
	for (auto typedStmtNum : typedStmtNums) {
		std::unordered_set<Ent> entities = rlStorage->getEnt(typedStmtNum);
		for (auto entity : entities) {
			std::string stmtNumString = to_string(typedStmtNum);
			res.push_back({ stmtNumString, paramString2 });
		}
	}
	return res;
}

std::vector<std::vector<std::string>> ModifiesUsesHandler::handleProcSynVar(Parameter fixedVarParam) {
	std::string fixedVar = fixedVarParam.getValue();
	std::vector<std::vector<std::string>> res;

	std::unordered_set<ProcName> procs = rlStorage->getProcsFromEnt(fixedVar);
	for (auto proc : procs) {
		res.push_back({ proc, fixedVar });
	}
	return res;
}

std::vector<std::vector<std::string>> ModifiesUsesHandler::handleProcSynWildcard() {
	std::vector<std::vector<std::string>> res;

	std::pair<std::vector<std::string>, std::vector<std::string>> procsEntities = rlStorage->getAllProcEntPairs();
	std::vector<std::string> procs = procsEntities.first;
	std::vector<std::string> entities = procsEntities.second;

	for (int i = 0; i < procs.size(); i++) {
		res.push_back({ procs[i], entities[i] });
	}
	return res;
}

std::vector<std::vector<std::string>> ModifiesUsesHandler::handle(Parameter param1, Parameter param2) {

	std::string paramString1 = param1.getValue();
	std::string paramString2 = param2.getValue();
	ParameterType param1Type = param1.getType();
	ParameterType param2Type = param2.getType();
	bool isIntParam1 = param1Type == ParameterType::FIXED_INT;
	bool isStringParam1 = param1Type == ParameterType::FIXED_STRING;
	bool isStringParam2 = param2Type == ParameterType::FIXED_STRING;
	bool isProcParam1 = param1Type == ParameterType::PROCEDURE;
	bool isWildcardParam1 = param1Type == ParameterType::WILDCARD;

	if (isIntParam1) {
		if (isStringParam2) {
			return handleIntVar(param1, param2);
		}
		else {
			return handleIntWildcard(param1);
		}
	}
	else if (isStringParam1) {
		if (isStringParam2) {
			return handleProcVar(param1, param2);
		}
		else {
			return handleProcWildcard(param1);
		}
	}
	else if (isProcParam1) {
		if (isStringParam2) {
			return handleProcSynVar(param2);
		}
		else {
			return handleProcSynWildcard();
		}
	}
	else {
		if (isStringParam2) {
			return handleStmtSynVar(param1, param2);
		}
		else {
			return handleStmtSynWildcard(param1, param2);
		}
	}
	return std::vector<std::vector<std::string>>();
}