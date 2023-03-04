#include "../../../spa/src/qps/entities/Parameter.h"
#include <string>

class With {
public:
    With(Parameter syn, std::string attrType, bool hasEquals, string equalsValue);
    Parameter syn;
    std::string attrType;
    bool hasEquals;
    std::string equalsValue;
};