#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <iostream>
#include <list>
#include <string>

// include your other headers here
#include "AbstractWrapper.h"
#include "PKB/PKB.h"
#include "PKB/ReadPKB.h"
#include "PKB/WritePKB.h"
#include "QPS/QPS.h"
#include "SP/SP.h"

class TestWrapper : public AbstractWrapper {
private:
    SP sourceProcessor;
    WritePKB writePKB;
    ReadPKB readPKB;
    PKB pkb;
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
