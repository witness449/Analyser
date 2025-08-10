#include "metric_impl/parameters_count.hpp"

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

namespace analyser::metric::metric_impl {
// здесь ваш код
MetricResult::ValueType CountParametersMetric ::CalculateImpl(const function::Function &f) const {
    auto res1 =
        std::ranges::views::split(f.ast, '\n') | std::views::transform([](auto &&rr) { return std::string_view{rr}; }) |
        std::views::drop_while([](auto &&str) { return !str.contains("parameters"); }) |
        std::views::take_while([](auto &&str) { return !(str.contains("body") || str.contains("return_type")); });
    /*std::vector<std::string_view> ress;
    for(auto r:res1){
        ress.push_back(std::string_view(r));
    }*/
    auto first = std::string_view{*res1.begin()};
    int space_count = first.find_first_of('p');
    std::string spaces;
    for (int i = 0; i < space_count + 2; ++i) {
        spaces.push_back(' ');
    }
    auto res2 = res1 | ranges::v3::view::filter([space_count, spaces](auto &&str) {
                    return str.substr(0, space_count + 2) == spaces && str[space_count + 3] != ' ';
                });
    return ranges::distance(res2);
}

std::string CountParametersMetric ::Name() const { return "count_parameters"; }

}  // namespace analyser::metric::metric_impl
