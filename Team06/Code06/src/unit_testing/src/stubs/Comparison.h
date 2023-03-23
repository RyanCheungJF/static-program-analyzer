#include <string>

#include "../../../spa/src/QPS/entities/Parameter.h"
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

private:
    Parameter leftParam;
    Parameter rightParam;
    ComparisonOperator op;
    static const unordered_map<string, ComparisonOperator> stringToOpMap;
};