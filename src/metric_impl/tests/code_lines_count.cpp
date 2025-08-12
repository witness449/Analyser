#include "metric_impl/code_lines_count.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

// здесь ваш код

class CodeLinesCountMetricTest : public testing::Test {
protected:
    CodeLinesCountMetric metric{};

    std::vector<function::Function> Get(const std::string &filename) {
        file::File file(filename);
        return function::FunctionExtractor{}.Get(file);
    }
};

TEST_F(CodeLinesCountMetricTest, MetricName) { ASSERT_EQ(metric.Name(), "count_of_lines"); }

TEST_F(CodeLinesCountMetricTest, LinesOfComments) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/comments.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 4);
}

TEST_F(CodeLinesCountMetricTest, LinesOfExceptions) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/exceptions.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 8);
}

TEST_F(CodeLinesCountMetricTest, LinesOfIf) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/if.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 4);
}

TEST_F(CodeLinesCountMetricTest, LinesOfLoops) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/loops.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 7);
}

TEST_F(CodeLinesCountMetricTest, LinesOfManyLines) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/many_lines.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 11);
}

TEST_F(CodeLinesCountMetricTest, LinesOfManyParameters) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/many_parameters.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 2);
}

TEST_F(CodeLinesCountMetricTest, LinesOfMatchCase) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/match_case.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 8);
}

TEST_F(CodeLinesCountMetricTest, LinesOfNestedIf) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/nested_if.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 9);
}

TEST_F(CodeLinesCountMetricTest, LinesOfSimple) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/simple.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 6);
}

TEST_F(CodeLinesCountMetricTest, LinesOfTernary) {
    function::Function f = Get("/workspaces/Analyser/src/metric_impl/tests/files/ternary.py").at(0);
    ASSERT_EQ(metric.Calculate(f).value, 2);
}

}  // namespace analyser::metric::metric_impl
