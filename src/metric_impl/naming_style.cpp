#include "metric_impl/naming_style.hpp"

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
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyser::metric::metric_impl {

MetricResult::ValueType NamingStyleMetric::CalculateImpl(const function::Function &f) const {
    const std::string &name = f.name;

    if (name.empty()) {
        return "unknown";
    }

    if (std::ranges::all_of(name, [](char c) { return std::islower(c); })) {
        return "lower case";
    } else if (std::ranges::all_of(name, [](char c) { return std::islower(c) || c == '_'; })) {
        return "snake case";
    } else if (std::islower(name[0]) && std::ranges::all_of(name.begin() + 1, name.end(), [](char c) {
                   return std::islower(c) || std::isupper(c);
               })) {
        return "camel case";
    } else if (std::isupper(name[0]) && std::ranges::all_of(name.begin() + 1, name.end(), [](char c) {
                   return std::islower(c) || std::isupper(c);
               })) {
        return "pascal case";
    } else {
        return "unknown";
    }
}

std::string NamingStyleMetric::Name() const { return "naming_style"; }

}  // namespace analyser::metric::metric_impl
