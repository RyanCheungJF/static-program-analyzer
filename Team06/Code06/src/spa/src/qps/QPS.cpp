#include "QPS.h"
// The main class used to instantiate and run the QPS


vector<string> QPS::processQueries(string queryString, ReadPKB& readPKB) {
    QPSParser parser = QPSParser();
    vector<Query> queryVec = parser.parse(queryString);
    vector<string> finalResult;
    for (Query query: queryVec) {
        // Some API call
        vector<string> result = query.evaluate(readPKB);
        finalResult.insert(finalResult.end(), result.begin(), result.end());
    }
    return finalResult;
}
