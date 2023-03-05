#include "../../qps/entities/Pattern.h"
#include "../storage/PatternStorage.h"

class IfWhilePatternHandler {
public:
    IfWhilePatternHandler(std::shared_ptr<PatternStorage> patternStorage);
    std::vector<std::vector<std::string>> handle(Pattern p);

private:
    std::shared_ptr<PatternStorage> patternStorage;
    std::vector<std::vector<std::string>> handleVar(Ent ent);
    std::vector<std::vector<std::string>> handleWildcard();
};