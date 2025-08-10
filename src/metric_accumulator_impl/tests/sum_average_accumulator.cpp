#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

// здесь ваш код

class SumAverageAccumulatorTest : public testing::Test {
protected:
    void SetUp() override {};
    void TearDown() override {};

    SumAverageAccumulator accumulator{};
    const double epsilon = 0.00001;
};

TEST_F(SumAverageAccumulatorTest, SingleMetric) {
    analyser::metric::MetricResult result{"single_metric", 1};
    accumulator.Reset();
    accumulator.Accumulate(result);
    accumulator.Finalize();
    ASSERT_NEAR(accumulator.Get().average, 1, epsilon);
    ASSERT_EQ(accumulator.Get().sum, 1);
}

TEST_F(SumAverageAccumulatorTest, MultipleMetric) {
    analyser::metric::MetricResult result1{"triple_metric", 1};
    analyser::metric::MetricResult result2{"triple_metric", 2};
    analyser::metric::MetricResult result3{"triple_metric", 3};
    accumulator.Reset();
    accumulator.Accumulate(result1);
    accumulator.Accumulate(result2);
    accumulator.Accumulate(result3);
    accumulator.Finalize();
    ASSERT_NEAR(accumulator.Get().average, 2, epsilon);
    ASSERT_EQ(accumulator.Get().sum, 6);
}

TEST_F(SumAverageAccumulatorTest, AccumulateReset) {
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
    ASSERT_NEAR(accumulator.Get().average, 2, epsilon);
    ASSERT_EQ(accumulator.Get().sum, 6);
}

TEST_F(SumAverageAccumulatorTest, AccumulateWithoutReset) {
    analyser::metric::MetricResult result{"single_metric", 1};
    analyser::metric::MetricResult result1{"triple_metric", 1};
    analyser::metric::MetricResult result2{"triple_metric", 2};
    analyser::metric::MetricResult result3{"triple_metric", 3};
    accumulator.Accumulate(result);
    accumulator.Finalize();
    ASSERT_THROW(accumulator.Accumulate(result1), std::runtime_error);
}

TEST_F(SumAverageAccumulatorTest, GetWithoutFinalize) {
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
