module;
#include <functional>
#include <map>
#include <source_location>
#include <string>
#include <vector>
export module CPPUnitTest:TestManager;

struct TestName {
    std::string group_name;
    std::string test_name;
    std::source_location location;

    bool operator<(const TestName &other) const {
        // Compare by group_name first, then by test_name
        return (group_name < other.group_name) ||
               (group_name == other.group_name && test_name < other.test_name);
    }
};

export class TestManager {
  public:
    static TestManager &instance() {
        static TestManager manager;
        return manager;
    }

    void add_test(const std::string &group_name, const std::string &test_name,
                  const bool multithreaded, std::function<void()> test,
                  const std::source_location location);

    void run_all_tests();

    TestManager() = default;
    ~TestManager() = default;

    TestManager(const TestManager &) = delete;
    TestManager &operator=(const TestManager &) = delete;

  private:
    std::map<TestName, std::function<void()>> st_tests;
    std::map<TestName, std::function<void()>> mt_tests;
    std::vector<TestName> failed_tests;
#if CPPUNITTEST_MAX_THREADS
    int max_concurrent_threads = CPPUNITTEST_MAX_THREADS;
#else
    int max_concurrent_threads = std::numeric_limits<int>::max();
#endif
    int max_group_width = 0;
    int max_test_width = 0;

    void run_all_st_tests();

    void run_all_mt_tests();
};
