#include "QPS.h"
// The main class used to instantiate and run the QPS

vector<string> QPS::processQueries(string queryString) {
    QPSParser parser = QPSParser();
    vector<Query> queryVec = parser.parse(queryString);
    vector<string> result;
    for (Query query: queryVec) {
        // Some API call
        result.push_back(query.process());
    }
    return result;
}