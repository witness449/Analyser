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

namespace mc = analyser::metric::metric_impl;
namespace ma = analyser::metric_accumulator::metric_accumulator_impl;

int main(int argc, char *argv[]) {
    try {
        analyser::cmd::ProgramOptions options;
        if (!options.Parse(argc, argv)) {
            throw;
        }

        analyser::metric::MetricExtractor metric_extractor;

        metric_extractor.RegisterMetric(std::make_unique<mc::CodeLinesCountMetric>(mc::CodeLinesCountMetric{}));
        metric_extractor.RegisterMetric(std::make_unique<mc::CountParametersMetric>(mc::CountParametersMetric{}));
        metric_extractor.RegisterMetric(
            std::make_unique<mc::CyclomaticComplexityMetric>(mc::CyclomaticComplexityMetric{}));

        auto analyseResults = analyser::AnalyseFunctions(options.GetFiles(), metric_extractor);

        std::ranges::for_each(analyseResults, [](const auto &res) {
            std::println("{}{}::{}", res.f.filename,
                         res.f.class_name.has_value() ? "::" + res.f.class_name.value() : "", res.f.name);
            std::ranges::for_each(res.mr, [](const auto &metric_result) {
                std::print("    {}: ", metric_result.metric_name);
                std::visit([](auto &&val) { std::println("{}", val); }, metric_result.value);
            });
        });

        analyser::metric_accumulator::MetricsAccumulator accumulator;
        accumulator.RegisterAccumulator("count_of_lines", std::make_unique<ma::SumAverageAccumulator>());
        accumulator.RegisterAccumulator("cyclomatic_complexity", std::make_unique<ma::SumAverageAccumulator>());
        accumulator.RegisterAccumulator("count_parameters", std::make_unique<ma::AverageAccumulator>());

        auto splitByFiles = SplitByFiles(analyseResults);

        std::ranges::for_each(splitByFiles, [&](const auto &splitted) {
            accumulator.ResetAccumulators();
            analyser::AccumulateFunctionAnalysis(splitted, accumulator);
            std::println("Accumulated analysis for file {}:", splitted[0].f.filename);

            auto &sumAvgCountLines = accumulator.GetFinalizedAccumulator<ma::SumAverageAccumulator>("count_of_lines");
            ma::SumAverageAccumulator::SumAverage resultCountLines = sumAvgCountLines.Get();
            std::println("    count_of_lines: sum={}, average={:.3f}", resultCountLines.sum, resultCountLines.average);

            auto &sumAvgCyclCompl =
                accumulator.GetFinalizedAccumulator<ma::SumAverageAccumulator>("cyclomatic_complexity");
            ma::SumAverageAccumulator::SumAverage resultCyclCompl = sumAvgCyclCompl.Get();
            std::println("    cyclomatic_complexity: sum={}, average={:.3f}", resultCyclCompl.sum,
                         resultCyclCompl.average);

            auto &avgCountPar = accumulator.GetFinalizedAccumulator<ma::AverageAccumulator>("count_parameters");
            double resultAvgCountPar = avgCountPar.Get();
            std::println("    count_parameters: average={:.3f}", resultAvgCountPar);
        });

        auto splitByClasses = SplitByClasses(analyseResults);

        std::ranges::for_each(splitByClasses, [&](const auto &splitted) {
            if (!splitted[0].f.class_name.has_value())
                return;
            accumulator.ResetAccumulators();
            analyser::AccumulateFunctionAnalysis(splitted, accumulator);
            std::println("Accumulated analysis for class {}:", splitted[0].f.class_name.value());

            auto &sumAvgCountLines = accumulator.GetFinalizedAccumulator<ma::SumAverageAccumulator>("count_of_lines");
            ma::SumAverageAccumulator::SumAverage resultCountLines = sumAvgCountLines.Get();
            std::println("    count_of_lines: sum={}, average={:.3f}", resultCountLines.sum, resultCountLines.average);

            auto &sumAvgCyclCompl =
                accumulator.GetFinalizedAccumulator<ma::SumAverageAccumulator>("cyclomatic_complexity");
            ma::SumAverageAccumulator::SumAverage resultCyclCompl = sumAvgCyclCompl.Get();
            std::println("    cyclomatic_complexity: sum={}, average={:.3f}", resultCyclCompl.sum,
                         resultCyclCompl.average);

            auto &avgCountPar = accumulator.GetFinalizedAccumulator<ma::AverageAccumulator>("count_parameters");
            double resultAvgCountPar = avgCountPar.Get();
            std::println("    count_parameters: average={:.3f}", resultAvgCountPar);
        });
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
