#include "TestWrapper.h"
#include "AbstractWrapper.h"
#include "DesignExtractor/DesignExtractor.h"
#include "SimpleParser/Parser.h"
#include "QPS/QueryEvaluator.h"
#include "QPS/QueryParser.h"
#include "QPS/Optimizer/QueryOptimizer.h"
#include "Common/FileReader.h"
#include <iostream>

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper *WrapperFactory::wrapper = 0;

AbstractWrapper *WrapperFactory::createWrapper() {
    if (wrapper == 0) wrapper = new TestWrapper;
    return wrapper;
}

// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

AbstractWrapper::~AbstractWrapper() = default;

// a default constructor
TestWrapper::TestWrapper() {
    // create any objects here as instance variables of this class
    // as well as any initialization required for your spa program
}

TestWrapper::~TestWrapper() = default;

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
    // call your parser to do the parsing
    // ...rest of your code...
    string fileContent = FileReader::getFileContent(filename);
    cout << "Loading source: " << filename << endl;
    Parser p;
    TNode *ast = p.parseProgram(fileContent);
    cout << "Parse complete" << endl;
    DesignExtractor designExtractor(ast, &pkbManager);
    bool isExtractionSuccess = designExtractor.extractDesign();
    delete ast;
    if (!isExtractionSuccess) { // exit entire program is there was problem with parsing or extracting
        cout << "Error with source code, exiting program" << endl;
        exit(0);
    }
    cout << "Extraction complete" << endl;
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string> &results) {
    // call your evaluator to evaluate the query here
    // ...code to evaluate query...
    cout << "Evaluating: " << query << endl;
    QueryParser qp = QueryParser{query};
    QueryObject *queryObject = qp.parse();
    QueryEvaluator queryEvaluator(&pkbManager);
    OptimizedQueryObject optimizedQueryObject = QueryOptimizer::create()
            .enableAllOptimizations(&pkbManager)
            .optimize(queryObject);
    QueryResult queryResult = queryEvaluator.evaluateQuery(&optimizedQueryObject);
    std::unordered_set<std::string> result = QueryProjector(queryResult, &pkbManager).getResult();
    // store the answers to the query in the results list (it is initially empty)
    // each result must be a string.
    std::copy(result.begin(), result.end(), std::back_inserter(results));

    delete queryObject;
    cout << "Done: " << query << endl;
}
