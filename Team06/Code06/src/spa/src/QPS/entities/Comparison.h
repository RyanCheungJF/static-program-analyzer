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
    Parameter getLeftParam();
    Parameter getRightParam();
    ComparisonOperator getOperator();
    vector<Parameter*> getAllUncheckedSynonyms();
    bool validateParams();
    static Comparison makeComparison(string o, Parameter leftP, Parameter rightP);

private:
    Parameter leftParam;
    Parameter rightParam;
    ComparisonOperator op;
    static const unordered_map<string, ComparisonOperator> stringToOpMap;
    static ComparisonOperator stringToOp(string s);
};

#endif // !SPA_QPS_COMPARISON_H