#ifndef SPA_QPS_COMPARISON_H
#define SPA_QPS_COMPARISON_H
#include "Parameter.h"
enum class ComparisonOperator {
    EQUALS,
};

class Comparison {
public:
    Comparison();
    Comparison(ComparisonOperator o, Parameter leftP, Parameter rightP);
    Parameter getLeftParam() const;
    Parameter getRightParam() const;
    ComparisonOperator getOperator() const;
    std::pair<ParameterType, ParameterType> getParameterTypes() const;
    std::pair<std::string, std::string> getParameterValues() const;
    std::pair<AttributeType, AttributeType> getParameterAttributes() const;
    vector<Parameter*> getAllUncheckedSynonyms();
    bool validateParams();
    bool hasValidComparisonTypes();
    static Comparison makeComparison(string o, Parameter leftP, Parameter rightP);
    bool operator==(const Comparison&) const;
    bool operator>(const Comparison&) const;
    bool operator<(const Comparison&) const;
    double getPriority();
    double calcPriority();

private:
    double evalPriority;
    Parameter leftParam;
    Parameter rightParam;
    ComparisonOperator op;
    static const unordered_map<string, ComparisonOperator> stringToOpMap;
    static ComparisonOperator stringToOp(string s);
};

#endif // !SPA_QPS_COMPARISON_H