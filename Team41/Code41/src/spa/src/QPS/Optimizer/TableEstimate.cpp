#include "TableEstimate.h"
#include "QPS/Optimizer/PKBAdapter.h"

#include <algorithm>
#include <iterator>
#include <unordered_set>

using namespace std;

TableEstimate::TableEstimate() = default;

TableEstimate::TableEstimate(PKBAdapter &pkbAdapter) : pkbAdapter(pkbAdapter) {}

void TableEstimate::merge(const vector<QueryDeclaration> &sch) {
  // merge with new schema
  unordered_set<QueryDeclaration, cvHasher, cvEquals> a(schema.begin(), schema.end());
  for (const QueryDeclaration &cv : sch) {
	if (a.find(cv) == a.end()) schema.push_back(cv);
  }

  // update row estimate
  estRows = estimateMergeCost(sch);
}

bool TableEstimate::hasCommonCol(const vector<QueryDeclaration> &sch) {
  unordered_set<QueryDeclaration, cvHasher, cvEquals> a(schema.begin(), schema.end());
  for (const QueryDeclaration &cv : sch) {
	if (a.find(cv) != a.end()) return true;
  }
  return false;
}

long long TableEstimate::getEstimatedRows() {
  return estRows;
}

vector<QueryDeclaration> TableEstimate::getSchema() {
  return schema;
}

long long TableEstimate::estimateMergeCost(const vector<QueryDeclaration> &newSch) {
  if (hasCommonCol(newSch)) {
	return max(estRows, pkbAdapter.getRowCount(newSch));
  } else {
	return estRows * pkbAdapter.getRowCount(newSch);
  }
}
