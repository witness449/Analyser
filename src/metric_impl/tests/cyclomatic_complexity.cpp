#include "metric_impl/cyclomatic_complexity.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {
// здесь ваш код

class CyclomaticComplexityMetricTest : public testing::Test {
protected:
    CyclomaticComplexityMetric metric{};

    std::vector<function::Function> Get(const std::string &filename) {
        file::File file(filename);
        return function::FunctionExtractor{}.Get(file);
    }
};

TEST_F(CyclomaticComplexityMetricTest, MetricName) { ASSERT_EQ(metric.Name(), "cyclomatic_complexity"); }

TEST_F(CyclomaticComplexityMetricTest, CyclComplOfComments) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/comments.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 0);
}

TEST_F(CyclomaticComplexityMetricTest, CyclComplOfExceptions) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/exceptions.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 4);
}

TEST_F(CyclomaticComplexityMetricTest, CyclComplOfIf) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/if.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 1);
}

TEST_F(CyclomaticComplexityMetricTest, CyclComplOfLoops) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/loops.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 3);
}

TEST_F(CyclomaticComplexityMetricTest, CyclComplOfManyLines) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/many_lines.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 1);
}

TEST_F(CyclomaticComplexityMetricTest, CyclComplOfManyParameters) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/many_parameters.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 1);
}

TEST_F(CyclomaticComplexityMetricTest, CyclComplOfMatchCase) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/match_case.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 4);
}

TEST_F(CyclomaticComplexityMetricTest, CyclComplOfNestedIf) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/nested_if.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 4);
}

TEST_F(CyclomaticComplexityMetricTest, CyclComplOfSimple) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/simple.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 1);
}

TEST_F(CyclomaticComplexityMetricTest, CyclComplOfTernary) {
    function::Function f = Get("/workspaces/Analyser/build/src/metric_impl/ternary.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 2);
}

}  // namespace analyser::metric::metric_impl
