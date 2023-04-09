#include "../../QPS/entities/Pattern.h"
#include "../storage/PatternStorage.h"
#include "PatternHandler.h"

class IfWhilePatternHandler : public PatternHandler {
public:
    std::vector<std::vector<std::string>> handle(Pattern& p);
    void setStorage(std::shared_ptr<PatternStorage>& patternStorage);

private:
    std::shared_ptr<PatternStorage> patternStorage;
    std::vector<std::vector<std::string>> handleVar(Ent ent);
    std::vector<std::vector<std::string>> handleWildcard();
};