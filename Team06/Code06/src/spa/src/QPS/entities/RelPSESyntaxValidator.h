#ifndef SPA_QPS_RELPSESYNTAXVALIDATOR_H
#define SPA_QPS_RELPSESYNTAXVALIDATOR_H

#include "Relationship.h"
#include "SyntaxValidator.h"

// Validates relationships with parameters of type Procedure/Stmt Entity
class RelPSESyntaxValidator : public SyntaxValidator<Relationship> {
public:
  bool validate(Relationship &);
};

#endif // !SPA_QPS_RELPSESYNTAXVALIDATOR_H