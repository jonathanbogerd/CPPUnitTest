module;
#include <algorithm>
#include <functional>
#include <source_location>
#include <sstream>
#include <string>
#include <vector>

module CPPUnitTest;

import :TestRegister;
import :TestManager;

TestRegister::TestRegister(const std::string &group_name, const std::string &test_name,
                           bool multithreaded, std::function<void()> test,
                           std::source_location location) {
    if (!is_group_excluded(group_name) && !is_test_excluded(test_name)) {
        TestManager::instance().add_test(group_name, test_name, multithreaded, std::move(test),
                                         location);
    }
}

bool TestRegister::is_group_excluded(const std::string &group_name) {
    const auto excluded_groups = get_excluded_list("EXCLUDED_GROUPS");
    return std::ranges::find(excluded_groups, group_name) != excluded_groups.end();
}

bool TestRegister::is_test_excluded(const std::string &test_name) {
    const auto excluded_tests = get_excluded_list("EXCLUDED_TESTS");
    return std::ranges::find(excluded_tests, test_name) != excluded_tests.end();
}

std::vector<std::string> TestRegister::get_excluded_list(const std::string &macro_name) {
    std::vector<std::string> result;

#ifdef EXCLUDED_GROUPS
    if (macro_name == "EXCLUDED_GROUPS") {
        return parse_macro_list(EXCLUDED_GROUPS, result);
    }
#endif

#ifdef EXCLUDED_TESTS
    if (macro_name == "EXCLUDED_TESTS") {
        return parse_macro_list(EXCLUDED_TESTS, result);
    }
#endif

    return {};
}

std::vector<std::string> TestRegister::parse_macro_list(const char *macro,
                                                        std::vector<std::string> &result) {
    std::istringstream stream(macro);
    std::string item;

    while (std::getline(stream, item, ',')) {
        result.emplace_back(trim(item));
    }

    return result;
}

std::string TestRegister::trim(const std::string &source) {
    std::string s(source);
    s.erase(0, s.find_first_not_of(" \n\r\t"));
    s.erase(s.find_last_not_of(" \n\r\t") + 1);
    return s;
}