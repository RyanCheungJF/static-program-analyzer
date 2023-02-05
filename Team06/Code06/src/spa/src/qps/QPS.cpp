#include "QPS.h"
// The main class used to instantiate and run the QPS

vector<string> QPS::processQueries(string queryString) {
    QPSParser parser = QPSParser();
    vector<Query> queryVec = parser.parse(queryString);
    vector<string> finalResult;
    for (Query query: queryVec) {
        // Some API call
        vector<string> result = query.evaluate();
        finalResult.insert(finalResult.end(), result.begin(), result.end());
    }
    return finalResult;
}