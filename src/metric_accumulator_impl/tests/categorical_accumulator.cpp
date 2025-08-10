#include "metric_accumulator_impl/categorical_accumulator.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

// здесь ваш код

class CategoricalAccumulatorTest : public testing::Test {
protected:
    void SetUp() override {};
    void TearDown() override {};

    CategoricalAccumulator accumulator{};
};

TEST_F(CategoricalAccumulatorTest, SingleCategory) {
    analyser::metric::MetricResult result{"single_metric", 1};
    accumulator.Reset();
    accumulator.Accumulate(result);
    accumulator.Finalize();
    auto accResult = accumulator.Get();
    ASSERT_EQ(accResult.size(), 1);
    ASSERT_EQ(accResult.at("1"), 1);
}

TEST_F(CategoricalAccumulatorTest, MultipleCategories) {
    analyser::metric::MetricResult result1{"triple_metric", 1};
    analyser::metric::MetricResult result2{"triple_metric", 3};
    analyser::metric::MetricResult result3{"triple_metric", 3};
    accumulator.Reset();
    accumulator.Accumulate(result1);
    accumulator.Accumulate(result2);
    accumulator.Accumulate(result3);
    accumulator.Finalize();
    auto accResult = accumulator.Get();
    ASSERT_EQ(accResult.size(), 2);
    ASSERT_EQ(accResult.at("1"), 1);
    ASSERT_EQ(accResult.at("3"), 2);
}

TEST_F(CategoricalAccumulatorTest, AccumulateReset) {
    analyser::metric::MetricResult result{"single_metric", 1};
    analyser::metric::MetricResult result1{"triple_metric", 3};
    analyser::metric::MetricResult result2{"triple_metric", 3};
    analyser::metric::MetricResult result3{"triple_metric", 3};
    accumulator.Accumulate(result);
    accumulator.Finalize();
    accumulator.Reset();
    accumulator.Accumulate(result1);
    accumulator.Accumulate(result2);
    accumulator.Accumulate(result3);
    accumulator.Finalize();
    auto accResult = accumulator.Get();
    ASSERT_EQ(accResult.size(), 1);
    ASSERT_EQ(accResult.at("3"), 3);
}

TEST_F(CategoricalAccumulatorTest, AccumulateWithoutReset) {
    analyser::metric::MetricResult result{"single_metric", 1};
    analyser::metric::MetricResult result1{"triple_metric", 1};
    analyser::metric::MetricResult result2{"triple_metric", 2};
    analyser::metric::MetricResult result3{"triple_metric", 3};
    accumulator.Accumulate(result);
    accumulator.Finalize();
    ASSERT_THROW(accumulator.Accumulate(result1), std::runtime_error);
}

TEST_F(CategoricalAccumulatorTest, GetWithoutFinalize) {
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
