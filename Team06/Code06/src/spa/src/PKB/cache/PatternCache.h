#include "../../QPS/entities/Pattern.h"
#include "Cache.h"
#include <iostream>

struct patternHash {
    std::size_t operator()(shared_ptr<Pattern> const& pattern) const {
        std::size_t h1 = std::hash<ParameterType>{}(pattern->getPatternSyn()->getType());
        std::size_t h2 = std::hash<std::string>{}(pattern->getEntRef()->getValue());
        std::size_t h3 = std::hash<std::string>{}(pattern->pattern);

        return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
    }
};

struct patternEquals {
    bool operator()(shared_ptr<Pattern> const& pattern1, shared_ptr<Pattern> const& pattern2) const {
        bool check1 = pattern1->getPatternSyn()->getType() == pattern2->getPatternSyn()->getType();
        bool check2 = pattern1->getEntRef()->getValue() == pattern2->getEntRef()->getValue();
        bool check3 = pattern1->pattern == pattern2->pattern;

        return check1 && check2 && check3;
    }
};

class PatternCache : public Cache<Pattern, std::vector<std::vector<std::string>>, patternHash, patternEquals> {};