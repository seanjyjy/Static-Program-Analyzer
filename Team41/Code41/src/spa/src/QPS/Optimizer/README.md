# Developer Guide
This guide is aimed to give you a better perspective of what class does what.

## Logical Structure
- `QueryOptimizer` - main point of entry for query optimization.
  - `ClauseDepGraph` - used to create the optimized query object
    - `SimpleGraph` - for general graph algorithms, e.g connected components
- `OptimizedQueryObject`
  - `QueryObject` - the original query object, for backwards compatibility
  - `ClauseGroups` - the collection of components of the clause dependency graph
    - `ClauseGroup` - a single components of the clause dependency graph
      - `TableEstimate` - each clause group maintains a table for dynamic optimization
        - `PKBAdapter` - to estimate the number of rows of a clause 
- `ClauseScorer` - for sorting clauses/clause groups. Scoring is intentionally decoupled from the clause.