#ifndef SPA_QPS_RELSSSYNTAXVALIDATOR_H
#define SPA_QPS_RELSSSYNTAXVALIDATOR_H

#include "SyntaxValidator.h"
#include "Relationship.h"

// Validates relationships with parameters of type Stmt Stmt
class RelSSSyntaxValidator : public SyntaxValidator<Relationship> {
public:
  bool validate(Relationship &);
};

#endif // !SPA_QPS_RELSSSYNTAXVALIDATOR_H