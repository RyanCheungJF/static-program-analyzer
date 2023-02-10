#include "ParentTStorage.h"

void ParentTStorage::write(std::vector<std::pair<StmtNum, StmtNum>> parentChildPairs) {
	for (std::pair<StmtNum, StmtNum> p : parentChildPairs) {
		StmtNum parent = p.first;
		StmtNum child = p.second;
		parentChildrenMap[parent].insert(child);
		childParentsMap[child].insert(parent);
	}
	return;
}

bool ParentTStorage::exists(StmtNum parent, StmtNum child) {
	std::unordered_map<StmtNum, std::unordered_set<StmtNum>>::const_iterator iter = childParentsMap.find(child);

	if (iter == childParentsMap.end()) return false;

	std::unordered_set<StmtNum> parents = iter->second;

	return parents.find(parent) != parents.end();
}

std::unordered_set<StmtNum> ParentTStorage::getRightWildcard(StmtNum parent) {
	std::unordered_map<StmtNum, std::unordered_set<StmtNum>>::const_iterator iter = parentChildrenMap.find(parent);

	if (iter == parentChildrenMap.end()) return std::unordered_set<StmtNum>();

	return iter->second;
}

std::unordered_set<StmtNum> ParentTStorage::getLeftWildcard(StmtNum child) {
	std::unordered_map<StmtNum, std::unordered_set<StmtNum>>::const_iterator iter = childParentsMap.find(child);

	if (iter == childParentsMap.end()) return std::unordered_set<StmtNum>();

	return iter->second;
}

std::pair<std::vector<StmtNum>, std::vector<StmtNum>> ParentTStorage::getAllPairs() {
	std::vector<StmtNum> parent;
	std::vector<StmtNum> children;

	for (auto parentChildrenPair : parentChildrenMap) {
		StmtNum parent = parentChildrenPair.first;
		std::unordered_set<StmtNum> children = parentChildrenPair.second;

		for (auto child : children) {
			parent.push_back(parent);
			children.push_back(child);
		}
	}
	return { parent, children };
}

