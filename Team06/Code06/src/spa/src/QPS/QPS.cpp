#include "QPS.h"
// The main class used to instantiate and run the QPS.

vector<string> QPS::processQueries(string queryString, ReadPKB &readPKB) {
  vector<string> finalResult;
  try {
    QPSParser parser = QPSParser();
    vector<Query> queryVec = parser.parse(queryString);
    for (Query query : queryVec) {
      // Some API call
      vector<string> result = query.evaluate(readPKB);
      finalResult.insert(finalResult.end(), result.begin(), result.end());
    }
  } catch (SyntaxException e) {
    return vector<string>{e.getMessage()};
  } catch (SemanticException e) {
    return vector<string>{e.getMessage()};
  } catch (InternalException e) {
    return vector<string>{e.getMessage()};
  }

  return finalResult;
}
