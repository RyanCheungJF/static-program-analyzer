#include "With.h"

With::With(Parameter syn, std::string attrType, bool hasEquals, string equalsValue) {
    this->syn = syn;
    this->attrType = attrType;
    this->hasEquals = hasEquals;
    this->equalsValue = equalsValue;
}