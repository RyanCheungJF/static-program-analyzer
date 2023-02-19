#include "TestWrapper.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
	if (wrapper == 0) wrapper = new TestWrapper;
	return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
	// create any objects here as instance variables of this class
	// as well as any initialization required for your spa program
	pkb.initializePkb();
	writePKB.setInstancePKB(pkb);
	readPKB.setInstancePKB(pkb);
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
    try {
        sourceProcessor.processFile(filename, &writePKB, &readPKB);
    } catch (SyntaxErrorException e) {
        AbstractWrapper::GlobalStop = true;
        throw e;
    } catch (SemanticErrorException e) {
        AbstractWrapper::GlobalStop = true;
        throw e;
    } catch (std::exception e) {
        AbstractWrapper::GlobalStop = true;
        throw e;
    }
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
    if (AbstractWrapper::GlobalStop) {
        return;
    }

	// call your evaluator to evaluate the query here
	  // ...code to evaluate query..
      vector<string> res = qps.processQueries(query, readPKB);
      for (std::string val :res) {
          results.push_back(val);
      }
	  // store the answers to the query in the results list (it is initially empty)
	  // each result must be a string.
}
