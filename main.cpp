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

    //Для отладки
    analyser::file::File file("/workspaces/Analyser/files/sample.py");
    std::vector<analyser::function::Function> funcVector;
    analyser::function::FunctionExtractor fExtract;
    std::vector<analyser::metric::MetricResults>resMetrics;


    funcVector=fExtract.Get(file);
    

    analyser::metric::MetricExtractor metric_extractor;
    
    metric_extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CodeLinesCountMetric>(analyser::metric::metric_impl::CodeLinesCountMetric{}));
    metric_extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CountParametersMetric>(analyser::metric::metric_impl::CountParametersMetric{}));
    metric_extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CyclomaticComplexityMetric>(analyser::metric::metric_impl::CyclomaticComplexityMetric{}));

    for(auto & f: funcVector){
        resMetrics.push_back(metric_extractor.Get(f));
    }


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
