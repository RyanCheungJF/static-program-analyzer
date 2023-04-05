#include "SharedPtrCompare.h"

template <class T> bool SharedPtrCompare::cmp(const shared_ptr<T>& a, const shared_ptr<T>& b) {
    return (*a) > (*b);
}

template bool SharedPtrCompare::cmp<Relationship>(const shared_ptr<Relationship>& a, const shared_ptr<Relationship>& b);