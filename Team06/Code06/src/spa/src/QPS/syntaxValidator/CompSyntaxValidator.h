#ifndef SPA_QPS_COMPSYNTAXVALIDATOR_H
#define SPA_QPS_COMPSYNTAXVALIDATOR_H

#include "../entities/Comparison.h"
#include "SyntaxValidator.h"

class CompSyntaxValidator : public SyntaxValidator<Comparison> {
public:
    CompSyntaxValidator();
    bool validate(Comparison&);
};

#endif // !SPA_QPS_COMPSYNTAXVALIDATOR_H