#include "metric_impl/cyclomatic_complexity.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <range/v3/all.hpp>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>

using namespace std::string_view_literals;

namespace analyser::metric::metric_impl {
// здесь ваш код

const std::unordered_set constructions = {"if_statement"sv,     "elif_clause"sv,           "while_statement"sv,
                                          "for_statement"sv,    "try_statement"sv,         "except_clause"sv,
                                          "finally_clause"sv,   "match_statement"sv,       "case_pattern"sv,
                                          "assert_statement"sv, "conditional_expression"sv};

MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function &f) const {
    auto res = f.ast | std::ranges::views::split('(') | std::views::transform([](auto &&str) {
                   auto sv = std::string_view{str};
                   return sv.substr(0, sv.find_first_of(" :"));
               }) |
               std::views::filter([](auto &&str) { return constructions.contains(std::string{str}); });
    std::cout << "cycl" << ranges::distance(res);
    return ranges::distance(res);
}

std::string CyclomaticComplexityMetric::Name() const { return "cyclomatic_complexity"; }

}  // namespace analyser::metric::metric_impl
