#ifndef SPA_QPS_PATTWOPARAMSYNTAXVALIDATOR_H
#define SPA_QPS_PATTWOPARAMSYNTAXVALIDATOR_H

#include "../entities/Pattern.h"
#include "SyntaxValidator.h"

class PatTwoParamSyntaxValidator : public SyntaxValidator<Pattern> {
public:
    bool validate(Pattern&);
    PatTwoParamSyntaxValidator();
};

#endif // !SPA_QPS_PATTWOPARAMSYNTAXVALIDATOR_H