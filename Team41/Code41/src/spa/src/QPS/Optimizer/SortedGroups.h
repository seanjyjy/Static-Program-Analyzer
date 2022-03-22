#pragma once

#include "AbstractGroups.h"
#include <functional>
#include <set>

struct abstractGroupComparator {
  bool operator()(const AbstractGroup &lhs,const AbstractGroup &rhs) const {
	return lhs.score() < rhs.score();
  }
};

class SortedGroups: public AbstractGroups {
 private:
  multiset<AbstractGroup, abstractGroupComparator>::iterator it;
  multiset<AbstractGroup, abstractGroupComparator> sortedGroups;
 public:
  SortedGroups();
  void addGroup(const AbstractGroup& group) override;
  SuperClause* pop() override;
  bool empty() override;
  const AbstractGroup &front() override;
};
