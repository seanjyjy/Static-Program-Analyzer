#include "QueryOptimizerBuilder.h"

QueryOptimizerBuilder::QueryOptimizerBuilder() = default;

QueryOptimizerBuilder &QueryOptimizerBuilder::setClauseGrouping(bool isOn) {
  queryOptimizer.isClauseGroupingEnabled = isOn;
  return *this;
}
QueryOptimizerBuilder &QueryOptimizerBuilder::setIntraGroupSort(bool isOn) {
  queryOptimizer.isIntraGroupSortEnabled = isOn;
  return *this;
}
QueryOptimizerBuilder &QueryOptimizerBuilder::setInterGroupSort(bool isOn) {
  queryOptimizer.isInterGroupSortEnabled = isOn;
  return *this;
}
QueryOptimizerBuilder &QueryOptimizerBuilder::enableDynamicPolling(const PKBAdapter &pkbAdapter) {
  queryOptimizer.adapter = pkbAdapter;
  queryOptimizer.isPkbAdapterSet = true;
  queryOptimizer.isDynamicPollingEnabled = true;
  return *this;
}
