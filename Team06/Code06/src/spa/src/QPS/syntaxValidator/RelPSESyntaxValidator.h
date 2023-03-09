#ifndef SPA_QPS_RELPSESYNTAXVALIDATOR_H
#define SPA_QPS_RELPSESYNTAXVALIDATOR_H

#include "SyntaxValidator.h"
#include "../entities/Relationship.h"

// Validates relationships with parameters of type Procedure/Stmt Entity
class RelPSESyntaxValidator : public SyntaxValidator<Relationship> {
public:
  bool validate(Relationship &);
  RelPSESyntaxValidator();
};

#endif // !SPA_QPS_RELPSESYNTAXVALIDATOR_H