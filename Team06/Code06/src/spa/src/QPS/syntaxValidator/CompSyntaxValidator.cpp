#include "CompSyntaxValidator.h"

CompSyntaxValidator::CompSyntaxValidator() {}

bool CompSyntaxValidator::validate(Comparison& c) {
    if (!c.hasValidComparisonTypes()) {
        throw SyntaxException();
    }
    return true;
}
