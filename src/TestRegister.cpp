module;
export module CPPUnitTest:TestRegister;
import std;
import :TestManager;

export class TestRegister {
  public:
    TestRegister(const std::string &group_name, const std::string &test_name,
                 const bool multithreaded, std::function<void()> test,
                 const std::source_location location = std::source_location::current()) {
        if (!is_group_excluded(group_name) && !is_test_excluded(test_name)) {
            TestManager::instance().add_test(group_name, test_name, multithreaded, std::move(test),
                                             location);
        }
    }

  private:
    static bool is_group_excluded(const std::string &group_name) {
        const auto excluded_groups = get_excluded_list("EXCLUDED_GROUPS");
        return std::ranges::find(excluded_groups, group_name) != excluded_groups.end();
    }

    static bool is_test_excluded(const std::string &test_name) {
        const auto excluded_tests = get_excluded_list("EXCLUDED_TESTS");
        return std::ranges::find(excluded_tests, test_name) != excluded_tests.end();
    }

    static std::vector<std::string> get_excluded_list(const std::string &macro_name) {
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

    static std::vector<std::string> parse_macro_list(const char *macro,
                                                     std::vector<std::string> &result) {
        std::istringstream stream(macro);
        std::string item;
        while (std::getline(stream, item, ',')) {
            result.emplace_back(trim(item));
        }
        return result;
    }

    static std::string trim(const std::string &source) {
        std::string s(source);
        s.erase(0, s.find_first_not_of(" \n\r\t"));
        s.erase(s.find_last_not_of(" \n\r\t") + 1);
        return s;
    }
};