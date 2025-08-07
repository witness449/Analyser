#include "metric_accumulator_impl/average_accumulator.hpp"

#include <unistd.h>

#include <algorithm>
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

namespace analyser::metric_accumulator::metric_accumulator_impl {

// здесь ваш код
void AverageAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    if (!is_finalized) {
        sum += metric_result.value;
        count++;
    }
}

void AverageAccumulator::Finalize() {
    if (!is_finalized) {
        average = static_cast<double>(sum) / count;
    }
    is_finalized = true;
}

void AverageAccumulator::Reset() {
    int sum = 0;
    int count = 0;
    double average = 0;
    is_finalized = false;
}

double AverageAccumulator::Get() const {
    if (is_finalized) {
        return average;
    }
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
