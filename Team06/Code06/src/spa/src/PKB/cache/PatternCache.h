#include <iostream>

#include "../../QPS/entities/Pattern.h"
#include "Cache.h"

struct patternHash {
    std::size_t operator()(shared_ptr<Pattern> const& pattern) const {
        std::size_t h1 = std::hash<ParameterType>{}(pattern->getPatternType());
        std::size_t h2 = std::hash<Parameter>{}(*pattern->getEntRef());
        std::size_t h3 = std::hash<std::string>{}(pattern->getExprSpecs()[0]);

        return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
    }
};

struct patternEquals {
    bool operator()(shared_ptr<Pattern> const& pattern1, shared_ptr<Pattern> const& pattern2) const {
        bool check1 = pattern1->getPatternType() == pattern2->getPatternType();
        bool check2 = *pattern1->getEntRef() == *pattern2->getEntRef();
        bool check3 = pattern1->getExprSpecs()[0] == pattern2->getExprSpecs()[0];

        return check1 && check2 && check3;
    }
};

class PatternCache : public Cache<Pattern, std::vector<std::vector<std::string>>, patternHash, patternEquals> {};
