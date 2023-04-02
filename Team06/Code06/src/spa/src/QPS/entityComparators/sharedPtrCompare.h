#ifndef SPA_QPS_SHAREDPTRCOMPARE_H
#define SPA_QPS_SHAREDPTRCOMPARE_H

#include <memory>
using namespace std;

class sharedPtrCompare {
public:
	template <class T> static const bool cmp(const shared_ptr<T>&  a, const shared_ptr<T>&  b);
};
#endif // !SPA_QPS_SHAREDPTRCOMPARE_H