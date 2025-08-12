#include "metric_accumulator.hpp"

#include <unistd.h>

#include <algorithm>
#include <any>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyser::metric_accumulator {

void MetricsAccumulator::AccumulateNextFunctionResults(const std::vector<metric::MetricResult> &metric_results) const {
    rs::for_each(metric_results, [&accumulators = accumulators](const auto &metric) {
        accumulators.at(metric.metric_name)->Accumulate(metric);
    });
}

void MetricsAccumulator::ResetAccumulators() {
    rs::for_each(accumulators | rv::values, [](auto &&acc) { acc->Reset(); });
}

}  // namespace analyser::metric_accumulator
