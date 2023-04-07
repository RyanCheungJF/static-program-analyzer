#include "../../QPS/entities/Pattern.h"
#include "Cache.h"

struct patternHash {
    std::size_t operator()(const Pattern& pattern) const {
        const Parameter& patternSyn = pattern.getPatternSyn();
        // variable v1, v2;
        // pattern a(v1, _) and pattern a(v2, _) should have the same hash in PKB's context.
        std::size_t h1 = std::hash<ParameterType>{}(pattern.getPatternType());
        std::size_t h2 = std::hash<std::string>{}(pattern.getExprSpecs()[0]);
        std::size_t h3 = std::hash<ParameterType>{}(pattern.getEntRefType());

        // only pattern a("x", _) and pattern a("x1", _) should have different hashes.
        if (patternSyn.isFixedStringType()) {
            std::size_t h3 = std::hash<Parameter>{}(pattern.getEntRef());
        }
        return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
    }
};

struct patternEquals {
    bool operator()(const Pattern& pattern1, const Pattern& pattern2) const {
        const Parameter& patternSyn1 = pattern1.getPatternSyn();
        const Parameter& patternSyn2 = pattern2.getPatternSyn();

        // variable v1, v2;
        // pattern a(v1, _) and pattern a(v2, _) should be equal in PKB's context.
        bool check1 = pattern1.getPatternType() == pattern2.getPatternType();
        bool check2 = pattern1.getExprSpecs()[0] == pattern2.getExprSpecs()[0];
        bool check3 = pattern1.getEntRefType() == pattern2.getEntRefType();

        // only pattern a("x", _) and pattern a("x1", _) should not be equals
        const Parameter& patternEnt1 = pattern1.getEntRef();
        if (patternEnt1.isFixedStringType()) {
            check3 = patternEnt1 == pattern2.getEntRef();
        }

        return check1 && check2 && check3;
    }
};

class PatternCache : public Cache<Pattern, std::vector<std::vector<std::string>>, patternHash, patternEquals> {};
