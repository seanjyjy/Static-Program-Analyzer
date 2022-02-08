#include "TestWrapper.h"
#include "DesignExtractor/DesignExtractor.h"
#include "SimpleParser/Parser.h"
#include "QPS/QueryEvaluator.h"
#include "QPS/QueryParser.h"
#include "Common/FileReader.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() : pkbManager() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
    // call your parser to do the parsing
  // ...rest of your code...
    string fileContent = FileReader::getFileContent(filename);
    Parser p;
    TNode* ast = p.parseProgram(fileContent);
    DesignExtractor designExtractor(ast, pkbManager);
    designExtractor.extractDesign();
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
  // call your evaluator to evaluate the query here
  // ...code to evaluate query...
    QueryParser qp = QueryParser{query};
    QueryObject* queryObject = qp.parse();
    QueryEvaluator queryEvaluator(pkbManager);
    std::unordered_set<std::string> result = queryEvaluator.evaluateQuery(queryObject);
    // store the answers to the query in the results list (it is initially empty)
    // each result must be a string.
    std::copy(result.begin(), result.end(), std::back_inserter(results));
}
