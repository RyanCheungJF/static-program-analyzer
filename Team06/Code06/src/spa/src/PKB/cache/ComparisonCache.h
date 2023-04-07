#include "../../QPS/entities/Comparison.h"
#include "Cache.h"

struct comparisonHash {
    std::size_t operator()(const Comparison& comp) const {
        Parameter& leftParam = comp.getLeftParam();
        Parameter& rightParam = comp.getRightParam();
        std::pair<ParameterType, ParameterType> paramTypes = comp.getParameterTypes();
        std::pair<std::string, std::string> paramValues = comp.getParameterValues();
        std::pair<AttributeType, AttributeType> attrTypes = comp.getParameterAttributes();

        std::size_t h1 = std::hash<ParameterType>{}(paramTypes.first);
        std::size_t h2 = std::hash<ParameterType>{}(paramTypes.second);
        std::size_t h3 = std::hash<ComparisonOperator>{}(comp.getOperator());

        if (leftParam.isFixedInt() || leftParam.isFixedStringType()) {
            std::size_t temp = std::hash<std::string>{}(paramValues.first);
            h1 = h1 ^ (temp << 1);
        }
        else {
            std::size_t temp = std::hash<AttributeType>{}(attrTypes.first);
            h1 = h1 ^ (temp << 1);
        }

        if (rightParam.isFixedInt() || rightParam.isFixedStringType()) {
            std::size_t temp = std::hash<std::string>{}(paramValues.second);
            h2 = h2 ^ (temp << 1);
        }
        else {
            std::size_t temp = std::hash<AttributeType>{}(attrTypes.second);
            h2 = h2 ^ (temp << 1);
        }

        return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
    }
};

struct comparisonEquals {
    bool operator()(const Comparison& comp1, const Comparison& comp2) const {
        std::pair<ParameterType, ParameterType> paramTypes1 = comp1.getParameterTypes();
        std::pair<ParameterType, ParameterType> paramTypes2 = comp2.getParameterTypes();
        std::pair<std::string, std::string> paramValues1 = comp1.getParameterValues();
        std::pair<std::string, std::string> paramValues2 = comp2.getParameterValues();
        std::pair<AttributeType, AttributeType> attrTypes1 = comp1.getParameterAttributes();
        std::pair<AttributeType, AttributeType> attrTypes2 = comp2.getParameterAttributes();

        bool check1 = paramTypes1.first == paramTypes2.first;
        bool check2 = paramTypes1.second == paramTypes2.second;
        bool check3 = comp1.getOperator() == comp2.getOperator();

        Parameter& leftParam1 = comp1.getLeftParam();
        Parameter& rightParam1 = comp1.getRightParam();

        if (leftParam1.isFixedInt() || leftParam1.isFixedStringType()) {
            check1 = check1 && paramValues1.first == paramValues2.first;
        }
        else {
            check1 = check1 && attrTypes1.first == attrTypes2.first;
        }

        if (rightParam1.isFixedInt() || rightParam1.isFixedStringType()) {
            check2 = check2 && paramValues1.second == paramValues2.second;
        }
        else {
            check2 = check2 && attrTypes1.second == attrTypes2.second;
        }

        return check1 && check2 && check3;
    }
};

class ComparisonCache
    : public Cache<Comparison, std::vector<std::vector<std::string>>, comparisonHash, comparisonEquals> {};
