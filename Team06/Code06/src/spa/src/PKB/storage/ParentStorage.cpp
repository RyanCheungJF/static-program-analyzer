#include "ParentStorage.h"

void ParentStorage::writeParent(StmtNum parent, StmtNum child) {
	if (parentChildMap.find(parent) != parentChildMap.end()) {
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

bool ParentStorage::checkParent(StmtNum parent, StmtNum child) {
	std::unordered_map<StmtNum, StmtNum>::const_iterator iter = childParentMap.find(child);
	return iter->second == parent;
}

std::unordered_set<StmtNum> ParentStorage::getChildren(StmtNum parent) {
	std::unordered_map<StmtNum, std::unordered_set<StmtNum>>::const_iterator iter = parentChildMap.find(parent);
	if (iter == parentChildMap.end()) return std::unordered_set<StmtNum>();
	return iter->second;
}

StmtNum ParentStorage::getParent(StmtNum child) {
	std::unordered_map<StmtNum, StmtNum>::const_iterator iter = childParentMap.find(child);
	if (iter == childParentMap.end()) return -1;
	return iter->second;
}

