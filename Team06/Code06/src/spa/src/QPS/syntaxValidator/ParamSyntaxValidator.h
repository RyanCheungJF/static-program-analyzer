#ifndef SPA_QPS_PARAMSYNTAXVALIDATOR_H
#define SPA_QPS_PARAMSYNTAXVALIDATOR_H

#include "../entities/Parameter.h"
#include "SyntaxValidator.h"

class ParamSyntaxValidator : public SyntaxValidator<Parameter> {
public:
    ParamSyntaxValidator();
    bool validate(Parameter&);
};

#endif // !SPA_QPS_PARAMSYNTAXVALIDATOR_H
