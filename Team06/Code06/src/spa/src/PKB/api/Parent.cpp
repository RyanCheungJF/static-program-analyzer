#include "Parent.h"

void Parent::setParent(StmtNum parent, StmtNum child) {
	store.writeParent(parent, child);
}

bool Parent::checkParent(StmtNum parent, StmtNum child) {
	return store.checkParent(parent, child);
}

std::vector<StmtNum> Parent::getChildren(StmtNum parent) {
	std::unordered_set childrenSet = store.getChildren(parent);

	std::vector<StmtNum> children = std::vector<StmtNum>();

	if (childrenSet.empty()) return children;

	children.reserve(childrenSet.size());
	for (auto it = childrenSet.begin(); it != childrenSet.end(); ) {
		children.push_back(std::move(childrenSet.extract(it++).value()));
	}

	return children;
}

StmtNum Parent::getParent(StmtNum child) {
	return store.getParent(child);
}