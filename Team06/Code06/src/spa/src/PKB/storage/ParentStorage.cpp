#include "ParentStorage.h"

void ParentStorage::write(StmtNum parent, StmtNum child) {
	if (parentChildMap.find(parent) == parentChildMap.end()) {
		std::unordered_set<StmtNum> childList;
		childList.insert(child);
		parentChildMap.insert({ parent, childList });
	} else {
		parentChildMap.at(parent).insert(child);
	}

	if (childParentMap.find(child) == childParentMap.end()) {
		childParentMap.insert({ child, parent });
	}
}

bool ParentStorage::exists(StmtNum parent, StmtNum child) {
	std::unordered_map<StmtNum, StmtNum>::const_iterator iter = childParentMap.find(child);

	if (iter == childParentMap.end()) return false;

	return iter->second == parent;
}

std::unordered_set<StmtNum> ParentStorage::getRightWildcard(StmtNum parent) {
	std::unordered_map<StmtNum, std::unordered_set<StmtNum>>::const_iterator iter = parentChildMap.find(parent);

	if (iter == parentChildMap.end()) return std::unordered_set<StmtNum>();

	return iter->second;
}

std::unordered_set<StmtNum> ParentStorage::getLeftWildcard(StmtNum child) {
	std::unordered_map<StmtNum, StmtNum>::const_iterator iter = childParentMap.find(child);

	if (iter == childParentMap.end()) return std::unordered_set<StmtNum>();

	return std::unordered_set<StmtNum>{ iter->second };
}

std::pair<std::vector<StmtNum>, std::vector<StmtNum>> ParentStorage::getAllPairs() {
	std::vector<StmtNum> parent;
	std::vector<StmtNum> children;
	for (auto childParentPair : childParentMap) {
		parent.push_back(childParentPair.second);
		children.push_back(childParentPair.first);
	}
	return { parent, children };
}

