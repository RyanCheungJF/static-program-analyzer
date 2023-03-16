#ifndef SPA_QPS_PATTHREEPARAMSYNTAXVALIDATOR_H
#define SPA_QPS_PATTHREEPARAMSYNTAXVALIDATOR_H

#include "../entities/Pattern.h"
#include "SyntaxValidator.h"

class PatThreeParamSyntaxValidator : public SyntaxValidator<Pattern> {
public:
    bool validate(Pattern&);
    PatThreeParamSyntaxValidator();
};

#endif // !SPA_QPS_PATTHREEPARAMSYNTAXVALIDATOR_H