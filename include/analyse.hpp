#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"

namespace analyser {

namespace rv = std::ranges::views;
namespace rs = std::ranges;

struct AnalyseFunctionsResult {
    analyser::function::Function f;
    analyser::metric::MetricResults mr;
};

std::vector<AnalyseFunctionsResult> AnalyseFunctions(const std::vector<std::string> &files,
                                                     const analyser::metric::MetricExtractor &metric_extractor) {
    // здесь ваш код
    auto res = rv::transform(files,
                             [](auto &&file) {
                                 auto functions = analyser::function::FunctionExtractor{}.Get(file::File{file});
                                 return functions;
                             }) |
               rv::join | rv::transform([&metric_extractor](auto &&function) {
                   auto metrics = metric_extractor.Get(function);
                   return AnalyseFunctionsResult{function, metrics};
               }) |
               rs::to<std::vector>();

    return res;
}

auto SplitByClasses(const auto &analysis) {
    auto res = analysis | rv::filter([](const auto &result) { return result.f.class_name.has_value(); }) |
               rv::chunk_by([](const auto &l, const auto &r) { return l.f.class_name == r.f.class_name; }) |
               rs::to<std::vector<std::vector<AnalyseFunctionsResult>>>();
}

auto SplitByFiles(const auto &analysis) {
    // здесь ваш код
    auto res = analysis | rv::chunk_by([](const auto &l, const auto &r) { return l.f.filename == r.f.filename; }) |
               rs::to<std::vector<std::vector<AnalyseFunctionsResult>>>();
}

void AccumulateFunctionAnalysis(const auto &analysis,
                                const analyser::metric_accumulator::MetricsAccumulator &accumulator) {
    // здесь ваш код
    rs::for_each(
        analysis, [&accumulator](const auto &mr) { accumulator.AccumulateNextFunctionResults(mr); },
        &AnalyseFunctionsResult::mr);
}

}  // namespace analyser
