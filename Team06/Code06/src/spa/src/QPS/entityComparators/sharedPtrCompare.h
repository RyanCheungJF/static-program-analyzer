#ifndef SPA_QPS_SHAREDPTRCOMPARE_H
#define SPA_QPS_SHAREDPTRCOMPARE_H

#include <memory>

#include "QPS/entities/Relationship.h"
using namespace std;

class SharedPtrCompare {
public:
    template <class T> static bool cmp(const shared_ptr<T>&, const shared_ptr<T>&);
};
#endif // !SPA_QPS_SHAREDPTRCOMPARE_H