#ifndef SPA_QPS_RELSSSYNTAXVALIDATOR_H
#define SPA_QPS_RELSSSYNTAXVALIDATOR_H

#include "../entities/Relationship.h"
#include "SyntaxValidator.h"

// Validates relationships with parameters of type Stmt Stmt
class RelSSSyntaxValidator : public SyntaxValidator<Relationship> {
public:
    bool validate(Relationship&);
    RelSSSyntaxValidator();
};

#endif // !SPA_QPS_RELSSSYNTAXVALIDATOR_H