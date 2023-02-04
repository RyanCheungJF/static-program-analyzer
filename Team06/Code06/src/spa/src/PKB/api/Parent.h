#include "../storage/ParentStorage.h"
#include <vector>

typedef int StmtNum;

class Parent {
public:
	void setParent(StmtNum parent, StmtNum child);
	bool checkParent(StmtNum parent, StmtNum child);
	std::vector<StmtNum> getChildren(StmtNum parent);
	StmtNum getParent(StmtNum child);
private:
	ParentStorage store;
};