#include "../../QPS/entities/Pattern.h"
#include "Cache.h"

struct patternHash {
    std::size_t operator()(shared_ptr<Pattern> const& pattern) const {
        // variable v1, v2;
        // pattern a(v1, _) and pattern a(v2, _) should have the same hash in PKB's context.
        std::size_t h1 = std::hash<ParameterType>{}(pattern->getPatternType());
        std::size_t h2 = std::hash<std::string>{}(pattern->getExprSpecs()[0]);
        std::size_t h3 = std::hash<ParameterType>{}(pattern->getEntRefType());

        // only pattern a("x", _) and pattern a("x1", _) should have different hashes.
        if (pattern->getPatternType() == ParameterType::FIXED_STRING) {
            std::size_t h4 = std::hash<Ent>{}(pattern->getEntRefValue());
            return ((((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1)) >> 1) ^ (h4 << 1);
        }
        return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
    }
};

struct patternEquals {
    bool operator()(shared_ptr<Pattern> const& pattern1, shared_ptr<Pattern> const& pattern2) const {
        // variable v1, v2;
        // pattern a(v1, _) and pattern a(v2, _) should be equal in PKB's context.
        bool check1 = pattern1->getPatternType() == pattern2->getPatternType();
        bool check2 = pattern1->getExprSpecs()[0] == pattern2->getExprSpecs()[0];

        // only pattern a("x", _) and pattern a("x1", _) should not be equals
        if (pattern1->getPatternType() == ParameterType::FIXED_STRING) {
            bool check3 = *pattern1->getEntRef() == *pattern2->getEntRef();
            return check1 && check2 && check3;
        }

        return check1 && check2;
    }
};

class PatternCache : public Cache<Pattern, std::vector<std::vector<std::string>>, patternHash, patternEquals> {};
