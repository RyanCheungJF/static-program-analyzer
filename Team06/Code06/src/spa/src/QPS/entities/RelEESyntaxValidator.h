#ifndef SPA_QPS_RELEESYNTAXVALIDATOR_H
#define SPA_QPS_RELEESYNTAXVALIDATOR_H

#include "Relationship.h"
#include "SyntaxValidator.h"

//Validates relationships with parameters of type Entity Entity
class RelEESyntaxValidator : public SyntaxValidator<Relationship> {
public:
  bool validate(Relationship &);
};

#endif // !SPA_QPS_RELEESYNTAXVALIDATOR_H