# Proxy

An Proxy class that facilitates as the logic center and provides an extra layer of abstraction to shield the client side
cache from other components beside the Query Evaluator. It acts as an intermediary to communicate with PKB and Cache for
the evaluators. This follows the proxy pattern to control the access of client mainly the Next and Affects interaction
with PKB.

The proxy have a composition relationship between PKB and the Cache

```
Proxy
└─ProxyrUtils
└─AffectsKBProxy
└─NextKBProxy
```

#### ProxyUtils

Provides common algorithms such as Breadth-first-search algorithm that is used in both Next and Affects

#### AffectsKBProxy

Act as a proxy for affects query that communicates with PKB and Cache. Depending on the storage status of either, it
will run some algorithm to fetch the relevant data and storage into the cache.

#### NextKBProxy

Act as a proxy for next query that communicates with PKB and Cache. Depending on the storage status of either, it will
run some algorithm to fetch the relevant data and storage into the cache.

#### Example Usage

```c++
string stmt1 = "1";
string stmt2 = "2";
// Checks if stmt2 is executed directly after stmt1
bool isNext = nextKBProxy->isNext(stmt1, stmt2);

// Retrieves a set of stmts that have stmts that are executed directly after
vector<string> setOfStmts = nextKBProxy->getAllStmtsThatHaveNextStmt();

string stmt1 = "1";
string stmt2 = "2";
// Checks if stmt1 directly or indirectly affects stmt2
bool isAffectsT = affectsKBProxy->isAffectsT(stmt1, stmt2);
```
