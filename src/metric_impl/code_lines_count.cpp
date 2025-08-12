#include "metric_impl/code_lines_count.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <range/v3/all.hpp>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

using namespace std::string_view_literals;

namespace analyser::metric::metric_impl {

MetricResult::ValueType CodeLinesCountMetric::CalculateImpl(const function::Function &f) const {
    auto res1 = std::ranges::views::split(f.ast, '\n') |
                std::views::transform([](auto &&str) { return std::string_view{str}; });
    auto res2 = res1 | ranges::v3::view::filter([](auto &&str) { return !str.contains("comment"); }) |
                ranges::v3::view::adjacent_filter([](auto &&str1, auto &&str2) {
                    auto s1 = str1.find_first_of('[');
                    auto s2 = str2.find_first_of('[');
                    auto e1 = str1.find_first_of(',');
                    auto e2 = str2.find_first_of(',');
                    auto a = std::string_view(str1.begin() + s1, str1.begin() + e1);
                    auto b = std::string_view(str2.begin() + s2, str2.begin() + e2);
                    return a != b;
                });
    return static_cast<int>(ranges::distance(res2));
}

std::string CodeLinesCountMetric::Name() const { return "count_of_lines"; }

}  // namespace analyser::metric::metric_impl
