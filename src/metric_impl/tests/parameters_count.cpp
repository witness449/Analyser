#include "metric_impl/parameters_count.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

// здесь ваш код

class CountParametersMetricTest : public testing::Test {
protected:
    CountParametersMetric metric{};

    std::vector<function::Function> Get(const std::string &filename) {
        file::File file(filename);
        return function::FunctionExtractor{}.Get(file);
    }
};

TEST_F(CountParametersMetricTest, MetricName) { ASSERT_EQ(metric.Name(), "count_parameters"); }

TEST_F(CountParametersMetricTest, CountParamOfComments) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/comments.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 3);
}

TEST_F(CountParametersMetricTest, CountParamOfExceptions) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/exceptions.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 0);
}

TEST_F(CountParametersMetricTest, CountParamOfIf) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/if.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 1);
}

TEST_F(CountParametersMetricTest, CountParamOfLoops) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/loops.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 1);
}

TEST_F(CountParametersMetricTest, CountParamOfManyLines) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/many_lines.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 0);
}

TEST_F(CountParametersMetricTest, CountParamOfManyParameters) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/many_parameters.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 5);
}

TEST_F(CountParametersMetricTest, CountParamOfMatchCase) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/match_case.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 1);
}

TEST_F(CountParametersMetricTest, CountParamOfNestedIf) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/nested_if.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 2);
}

TEST_F(CountParametersMetricTest, CountParamOfSimple) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/simple.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 0);
}

TEST_F(CountParametersMetricTest, CountParamOfTernary) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/ternary.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 1);
}

}  // namespace analyser::metric::metric_impl
