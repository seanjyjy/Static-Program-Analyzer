#pragma once

#include "AbstractGroup.h"

class AbstractGroups {
 public:
  virtual void addGroup(const AbstractGroup& group) = 0;
  virtual const AbstractGroup &front() = 0;
  virtual SuperClause* pop() = 0;
  virtual bool empty() = 0;
};
