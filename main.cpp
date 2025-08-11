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

#include "analyse.hpp"
#include "cmd_options.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"
#include "metric_accumulator_impl/accumulators.hpp"
#include "metric_impl/metrics.hpp"

int main(int argc, char *argv[]) {
    analyser::cmd::ProgramOptions options;
    options.Parse(argc, argv);
    // распарсите входные параметры

    // Для отладки
    // analyser::file::File file("/workspaces/Analyser/files/sample.py");
    /*std::vector<analyser::function::Function> funcVector;
    analyser::function::FunctionExtractor fExtract;
    std::vector<analyser::metric::MetricResults> resMetrics;

    funcVector = fExtract.Get(file);*/

    analyser::metric::MetricExtractor metric_extractor;

    metric_extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CodeLinesCountMetric>(
        analyser::metric::metric_impl::CodeLinesCountMetric{}));
    metric_extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CountParametersMetric>(
        analyser::metric::metric_impl::CountParametersMetric{}));
    metric_extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CyclomaticComplexityMetric>(
        analyser::metric::metric_impl::CyclomaticComplexityMetric{}));

    /*for (auto &f : funcVector) {
        resMetrics.push_back(metric_extractor.Get(f));
    }*/

    auto analyseResults = analyser::AnalyseFunctions(options.GetFiles(), metric_extractor);

    std::ranges::for_each(analyseResults, [](const auto &res) {
        std::println("{}{}::{}", res.f.filename, res.f.class_name.has_value() ? "::" + res.f.class_name.value() : "",
                     res.f.name);
        std::ranges::for_each(res.mr, [](const auto &metric_result) {
            std::println("    {}: {}", metric_result.metric_name, metric_result.value);
        });
    });

    analyser::metric_accumulator::MetricsAccumulator accumulator;
    accumulator.RegisterAccumulator(
        "count_of_lines",
        std::make_unique<analyser::metric_accumulator::metric_accumulator_impl::SumAverageAccumulator>());
    accumulator.RegisterAccumulator(
        "cyclomatic_complexity",
        std::make_unique<analyser::metric_accumulator::metric_accumulator_impl::SumAverageAccumulator>());
    accumulator.RegisterAccumulator(
        "count_parameters",
        std::make_unique<analyser::metric_accumulator::metric_accumulator_impl::AverageAccumulator>());

    auto splitByFiles = SplitByFiles(analyseResults);

    /*std::vector<std::vector<AnalyseFunctionsResult>>
    struct AnalyseFunctionsResult {
        analyser::function::Function f;
        analyser::metric::MetricResults mr;
    };
    std::vector<MetricResult>;*/

    std::ranges::for_each(splitByFiles, [&](const auto &splitted) {
        accumulator.ResetAccumulators();
        analyser::AccumulateFunctionAnalysis(splitted, accumulator);
        std::println("Accumulated analysis for file {}:", splitted[0].f.filename);

        auto &sumAvgCountLines =
            accumulator
                .GetFinalizedAccumulator<analyser::metric_accumulator::metric_accumulator_impl::SumAverageAccumulator>(
                    "count_of_lines");
        analyser::metric_accumulator::metric_accumulator_impl::SumAverageAccumulator::SumAverage result =
            sumAvgCountLines.Get();
        std::println("    count_of_lines: sum={}, average={}", result.sum, result.average);
    });

    auto splitByClasses = SplitByClasses(analyseResults);

    std::ranges::for_each(splitByClasses, [&](const auto &splitted) {
        if (!splitted[0].f.class_name.has_value())
            return;
        accumulator.ResetAccumulators();
        analyser::AccumulateFunctionAnalysis(splitted, accumulator);
        std::println("Accumulated analysis for class {}:", splitted[0].f.class_name.value());

        auto &sumAvgCountLines =
            accumulator
                .GetFinalizedAccumulator<analyser::metric_accumulator::metric_accumulator_impl::SumAverageAccumulator>(
                    "count_of_lines");
        analyser::metric_accumulator::metric_accumulator_impl::SumAverageAccumulator::SumAverage result =
            sumAvgCountLines.Get();
        std::println("    count_of_lines: sum={}, average={}", result.sum, result.average);
    });

    // зарегистрируйте метрики в metric_extractor

    // запустите analyser::AnalyseFunctions
    // выведете результаты анализа на консоль

    // analyser::metric_accumulator::MetricsAccumulator accumulator;
    // зарегистрируйте аккумуляторы метрик в accumulator

    // запустите analyser::SplitByFiles
    // запустите analyser::AccumulateFunctionAnalysis для каждого подмножества результатов метрик
    // выведете результаты на консоль

    // запустите analyser::SplitByClasses
    // запустите analyser::AccumulateFunctionAnalysis для каждого подмножества результатов метрик
    // выведете результаты на консоль

    // запустите analyser::AccumulateFunctionAnalysis для всех результатов метрик
    // выведете результаты на консоль

    return 0;
}
