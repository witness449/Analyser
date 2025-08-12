#include "metric_impl/naming_style.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

class NamingStyleMetricTest : public testing::Test {
protected:
    NamingStyleMetric metric{};

    std::vector<function::Function> Get(const std::string &filename) {
        file::File file(filename);
        return function::FunctionExtractor{}.Get(file);
    }
};

TEST_F(NamingStyleMetricTest, MetricName) { ASSERT_EQ(metric.Name(), "naming_style"); }

TEST_F(NamingStyleMetricTest, namingStyleCommentsComments) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/comments.py").at(0);
    ASSERT_EQ(std::get<std::string>(metric.Calculate(f).value), "unknown");
}

TEST_F(NamingStyleMetricTest, NamingStyleExceptions) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/exceptions.py").at(0);
    ASSERT_EQ(std::get<std::string>(metric.Calculate(f).value), "unknown");
}

TEST_F(NamingStyleMetricTest, NamingStyleIf) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/if.py").at(0);
    ASSERT_EQ(std::get<std::string>(metric.Calculate(f).value), "camel case");
}

TEST_F(NamingStyleMetricTest, NamingStyleLoops) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/loops.py").at(0);
    ASSERT_EQ(std::get<std::string>(metric.Calculate(f).value), "pascal case");
}

TEST_F(NamingStyleMetricTest, NamingStyleManyLines) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/many_lines.py").at(0);
    ASSERT_EQ(std::get<std::string>(metric.Calculate(f).value), "lower case");
}

TEST_F(NamingStyleMetricTest, NamingStyleManyParameters) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/many_parameters.py").at(0);
    ASSERT_EQ(std::get<std::string>(metric.Calculate(f).value), "snake case");
}

TEST_F(NamingStyleMetricTest, NamingStyleMatchCase) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/match_case.py").at(0);
    ASSERT_EQ(std::get<std::string>(metric.Calculate(f).value), "unknown");
}

TEST_F(NamingStyleMetricTest, NamingStyleNestedIf) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/nested_if.py").at(0);
    ASSERT_EQ(std::get<std::string>(metric.Calculate(f).value), "pascal case");
}

TEST_F(NamingStyleMetricTest, NamingStyleSimple) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/simple.py").at(0);
    ASSERT_EQ(std::get<std::string>(metric.Calculate(f).value), "snake case");
}

TEST_F(NamingStyleMetricTest, NamingStyleTernary) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/ternary.py").at(0);
    ASSERT_EQ(std::get<std::string>(metric.Calculate(f).value), "unknown");
}

}  // namespace analyser::metric::metric_impl
