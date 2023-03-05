#include <string>

#include "../../../spa/src/qps/entities/Parameter.h"

class With {
public:
    With(Parameter syn, std::string attrType, bool hasEquals, string equalsValue);
    Parameter syn;
    std::string attrType;
    bool hasEquals;
    std::string equalsValue;
};