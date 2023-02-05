#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>

// include your other headers here
#include "AbstractWrapper.h"
#include "SP.h"
#include "PKB/PKB.h"
#include "PKB/ReadPKB.h"
#include "qps/QPS.h"

class TestWrapper : public AbstractWrapper {
private:
	SP sourceProcessor;
	WritePKB writePKB;
    ReadPKB readPKB;
	PKB pkb;
	Follows f;
	EntityStorage et;
	ProcedureStorage ps;
	StatementAPI st;
    QPS qps;

 public:
  // default constructor
  TestWrapper();
  
  // destructor
  ~TestWrapper();
  
  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);
  
  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);
};

#endif
