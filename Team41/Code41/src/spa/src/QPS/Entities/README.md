# Entities

```
Entities
└─EntitiesType/
└─Readers/
```

#### EntitiesType

Represents the different entities used in the Source's query language.

Examples would be like: `stmt`, `read`, `call`, etc

#### Readers

Based on different entities, each entities will have a store a reader as a composition relationship. These readers are
to facilitate to communicate with the PKB to get relevant information regarding a certain entity. This is to allow
better OCP.

#### Example Usage

```c++
Entities *stmtEntity = new StmtEntities(); // creates a stmt entities

stmtEntity->isStmt(); // returns true
stmtEntity->isCall(); // returns false

EntitiesReader *reader = stmtEntity->getReader(); // gets the relevant stmt reader
unordered_set<string> result = reader->getEntities(pkb); // retrieves all stmts
```

