#ifndef SPA_QPS_SYNTAXVALIDATOR_H
#define SPA_QPS_SYNTAXVALIDATOR_H
template <class T>

class SyntaxValidator {
public:
    virtual bool validate(T& val) = 0;
};
#endif // !SPA_QPS_SYNTAXVALIDATOR_H