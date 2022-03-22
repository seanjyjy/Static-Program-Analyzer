#pragma once

#include "QueryOptimizer.h";

class QueryOptimizerBuilder {
 private:
  QueryOptimizer queryOptimizer;
 public:
  QueryOptimizerBuilder();

  // allow static casting of the builder to QueryOptimizer
  explicit operator QueryOptimizer() const { return queryOptimizer; }

  QueryOptimizerBuilder& setClauseGrouping(bool isOn);
  QueryOptimizerBuilder& setIntraGroupSort(bool isOn);
  QueryOptimizerBuilder& setInterGroupSort(bool isOn);
  QueryOptimizerBuilder& enableDynamicPolling(const PKBAdapter& pkbAdapter);
};
