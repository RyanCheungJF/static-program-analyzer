#include "../storage/StmtStmtRLStorage.h"
#include "../../qps/entities/Parameter.h"
#include "../storage/StmtStorage.h"
#include <algorithm>


class StmtStmtRLHandler {
public:
	StmtStmtRLHandler(StmtStmtRLStorage* rlStorage, StmtStorage* stmtStorage);
	std::vector<std::vector<std::string>> handle(Parameter param1, Parameter param2);
private:
	StmtStmtRLStorage* rlStorage = NULL;
	StmtStorage* stmtStorage = NULL;

	// e.g. FollowsT(1, 2)
	std::vector<std::vector<std::string>> handleIntInt(Parameter param1, Parameter param2);

	// e.g. FollowsT(1, if)
	std::vector<std::vector<std::string>> handleIntStmt(Parameter param1, Parameter param2);

	// e.g. FollowsT(1, _)
	std::vector<std::vector<std::string>> handleIntWildcard(Parameter param1, Parameter param2);

	// e.g. FollowsT(a, 2)
	std::vector<std::vector<std::string>> handleStmtInt(Parameter param1, Parameter param2);

	// e.g. FollowsT(a, while)
	std::vector<std::vector<std::string>> handleStmtStmt(Parameter param1, Parameter param2);

	// e.g. FollowsT(a, _)
	std::vector<std::vector<std::string>> handleStmtWildcard(Parameter param1, Parameter param2);

	// e.g. FollowsT(_, 2)
	std::vector<std::vector<std::string>> handleWildcardInt(Parameter param1, Parameter param2);

	// .e.g FollowsT(_, a)
	std::vector<std::vector<std::string>> handleWildcardStmt(Parameter param1, Parameter param2);

	// e.g. FollowsT(_, _)
	std::vector<std::vector<std::string>> handleWildcardWildcard(Parameter param1, Parameter param2);
};