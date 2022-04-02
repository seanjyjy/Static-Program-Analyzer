# Adapters 

An Adapter class that facilitates as the logic center and provides an extra layer of abstraction to 
shield the client side cache from other components beside the Query Evaluator. It acts as an intermediary to 
communicate with PKB and Cache for the evaluators.

The adapters have a composition relationship between PKB and the Cache

```
Adapters
└─AdapterUtils
└─AffectsKBAdapter
└─NextKBAdapter
```

#### AdapterUtils

Provides common algorithms such as Breadth-first-search algorithm that is used in both Next and Affects

#### AffectsKBAdapter

Act as a logic center for affects query that communicates with PKB and Cache. Depending on the storage 
status of either, it will run some algorithm to fetch the relevant data and storage into the cache.

#### NextKBAdapter

Act as a logic center for next query that communicates with PKB and Cache. Depending on the storage
status of either, it will run some algorithm to fetch the relevant data and storage into the cache.

#### Example Usage

```c++
string stmt1 = "1";
string stmt2 = "2";
// Checks if stmt2 is executed directly after stmt1
bool isNext = nextKBAdapter->isNext(stmt1, stmt2);

// Retrieves a set of stmts that have stmts that are executed directly after
vector<string> setOfStmts = nextKBAdapter->getAllStmtsThatHaveNextStmt();

string stmt1 = "1";
string stmt2 = "2";
// Checks if stmt1 directly or indirectly affects stmt2
bool isAffectsT = affectsKBAdapter->isAffectsT(stmt1, stmt2);
```
