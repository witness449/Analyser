#include "metric_accumulator_impl/average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

class AverageAccumulatorTest : public testing::Test {
protected:
    void SetUp() override {};
    void TearDown() override {};

    AverageAccumulator accumulator{};
    const double epsilon = 0.00001;
};

TEST_F(AverageAccumulatorTest, SingleMetric) {
    analyser::metric::MetricResult result{"single_metric", 1};
    accumulator.Reset();
    accumulator.Accumulate(result);
    accumulator.Finalize();
    ASSERT_NEAR(accumulator.Get(), 1, epsilon);
}

TEST_F(AverageAccumulatorTest, MultipleMetric) {
    analyser::metric::MetricResult result1{"triple_metric", 1};
    analyser::metric::MetricResult result2{"triple_metric", 2};
    analyser::metric::MetricResult result3{"triple_metric", 3};
    accumulator.Reset();
    accumulator.Accumulate(result1);
    accumulator.Accumulate(result2);
    accumulator.Accumulate(result3);
    accumulator.Finalize();
    ASSERT_NEAR(accumulator.Get(), 2, epsilon);
}

TEST_F(AverageAccumulatorTest, AccumulateReset) {
    analyser::metric::MetricResult result{"single_metric", 1};
    analyser::metric::MetricResult result1{"triple_metric", 1};  // Проверка на имя!
    analyser::metric::MetricResult result2{"triple_metric", 2};
    analyser::metric::MetricResult result3{"triple_metric", 3};
    accumulator.Accumulate(result);
    accumulator.Finalize();
    accumulator.Reset();
    accumulator.Accumulate(result1);
    accumulator.Accumulate(result2);
    accumulator.Accumulate(result3);
    accumulator.Finalize();
    ASSERT_NEAR(accumulator.Get(), 2, epsilon);
}

TEST_F(AverageAccumulatorTest, AccumulateWithoutReset) {
    analyser::metric::MetricResult result{"single_metric", 1};
    analyser::metric::MetricResult result1{"triple_metric", 1};
    analyser::metric::MetricResult result2{"triple_metric", 2};
    analyser::metric::MetricResult result3{"triple_metric", 3};
    accumulator.Accumulate(result);
    accumulator.Finalize();
    ASSERT_THROW(accumulator.Accumulate(result1), std::runtime_error);
}

TEST_F(AverageAccumulatorTest, GetWithoutFinalize) {
    analyser::metric::MetricResult result1{"triple_metric", 1};
    analyser::metric::MetricResult result2{"triple_metric", 2};
    analyser::metric::MetricResult result3{"triple_metric", 3};
    accumulator.Reset();
    accumulator.Accumulate(result1);
    accumulator.Accumulate(result2);
    accumulator.Accumulate(result3);
    ASSERT_THROW(accumulator.Get(), std::runtime_error);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
