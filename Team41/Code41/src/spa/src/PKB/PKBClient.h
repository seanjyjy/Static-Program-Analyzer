#pragma once

#include <vector>
#include "PKB/Clients/PKBBaseClient.h"
#include "PKB/Clients/PKBAbstractionClient.h"
#include "PKB/Clients/PKBPatternClient.h"
#include "PKB/Clients/PKBCFGClient.h"
#include "Common/AstNode/TNode.h"

using namespace std;

/**
 * Used for retrieving knowledge base on the parsed SIMPLE source code.
 *
 * This PKB facade class is a convenient class that extends from various mini facades.
 * It is used mainly by the query client and provides methods to
 * access the metadata and abstractions stored in various relational tables.
 */
class PKBClient :
        public PKBBaseClient,
        public PKBAbstractionClient,
        public PKBPatternClient,
        public PKBCFGClient {
};