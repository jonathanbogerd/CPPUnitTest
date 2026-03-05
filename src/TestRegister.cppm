// TestRegister.cppm
module;
#include <functional>
#include <source_location>
#include <string>
#include <vector>

export module CPPUnitTest:TestRegister;

export class TestRegister {
public:
    TestRegister(const std::string& group_name,
                 const std::string& test_name,
                 bool multithreaded,
                 std::function<void()> test,
                 std::source_location location = std::source_location::current());
private:
    static bool is_group_excluded(const std::string& group_name);
    static bool is_test_excluded(const std::string& test_name);
    static std::vector<std::string> get_excluded_list(const std::string& macro_name);
    static std::vector<std::string> parse_macro_list(const char* macro,
                                                     std::vector<std::string>& result);
    static std::string trim(const std::string& source);
};