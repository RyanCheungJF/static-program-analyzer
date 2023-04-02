#include "sharedPtrCompare.h"

template<class T> const bool sharedPtrCompare::cmp(const shared_ptr<T>&  a, const shared_ptr<T>&  b)
{
	return (*a) > (*b);
}