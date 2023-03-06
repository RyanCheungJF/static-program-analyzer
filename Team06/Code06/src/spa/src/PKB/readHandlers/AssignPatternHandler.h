#include "../../QPS/entities/Parameter.h"
#include "../../QPS/entities/Pattern.h"
#include "../storage/PatternWithExprStorage.h"
#include "../utils/utils.h"

class AssignPatternHandler {
public:
    AssignPatternHandler(std::shared_ptr<PatternWithExprStorage> patternStorage);

    std::vector<std::vector<std::string>> handle(Pattern p);

private:
    std::shared_ptr<PatternWithExprStorage> patternStorage;

    // pattern a ("a", _)
    std::vector<std::vector<std::string>> handleVarWildcard(std::string lhs);

    // pattern a ("a", "v") OR pattern a ("a", _"v"_)
    std::vector<std::vector<std::string>> handleVarPattern(std::string lhs, std::string rhs,
                                                           bool (*checkTree)(Expression*, Expression*));

    // pattern a (v, _) OR pattern a (_, _)
    std::vector<std::vector<std::string>> handleWildcardWildcard();

    // pattern a (v, "v") OR pattern a (v, _"v"_)
    std::vector<std::vector<std::string>> handleSynPattern(std::string rhs,
                                                           bool (*checkTree)(Expression*, Expression*));

    // pattern a (_, "v") OR pattern a (_, _"v"_)
    std::vector<std::vector<std::string>> handleWildcardPattern(std::string rhs,
                                                                bool (*checkTree)(Expression*, Expression*));
};