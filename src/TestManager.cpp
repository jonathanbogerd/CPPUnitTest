module;
export module CPPUnitTest:TestManager;
import std;
const std::string GREEN = "\033[1;32m";
const std::string BLUE = "\033[1;34m";
const std::string RED = "\033[1;31m";
const std::string RESET = "\033[0m";

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

void print_running_test(const TestName &name, const int group_width, const int test_width) {
    std::cout << BLUE << std::setw(15) << "[ RUNNING ]" << RESET
              << " Group: " << std::setw(group_width) << name.group_name
              << " | Test: " << std::setw(test_width) << name.test_name << '\n';
}

void print_test_passed(const TestName &name, const std::chrono::microseconds &duration,
                       const int group_width, const int test_width) {
    std::cout << GREEN << std::setw(15) << "[   PASSED ]" << RESET
              << " Group: " << std::setw(group_width) << name.group_name
              << " | Test: " << std::setw(test_width) << name.test_name
              << " | Duration: " << duration.count() << std::setw(5) << " µs\n";
}

void print_test_failed(const TestName &name, const std::exception &exc, const int group_width,
                       const int test_width) {
    std::cerr << RED << std::setw(15) << "[   FAILED ]"
              << " Group: " << std::setw(group_width) << name.group_name
              << " | Test: " << std::setw(test_width) << name.test_name
              << " | Reason: " << exc.what() << RESET << '\n';
}

void print_results(const std::vector<TestName> &failed_tests,
                   const std::chrono::microseconds &duration, const int total_tests,
                   const int group_width, const int test_width) {
    std::cout << "Finished testing, took " << duration.count() << " microseconds \n";
    std::cout << failed_tests.size() << " out of " << total_tests << " failed\n";
    if (failed_tests.size() > 0) {
        std::cout << "The following tests failed:\n";
    }
    for (const TestName &name: failed_tests) {
        std::cout << RED << std::setw(15) << "[   FAILED ]" << RESET
                  << " Group: " << std::setw(group_width) << name.group_name
                  << " | Test: " << std::setw(test_width) << name.test_name
                  << " | File: " << name.location.file_name() << "(" << name.location.line() << ":"
                  << name.location.column() << ")" << '\n';
    }
}

export class TestManager {
  public:
    static TestManager &instance() {
        static TestManager manager;
        return manager;
    }

    void add_test(const std::string &group_name, const std::string &test_name,
                  const bool multithreaded, std::function<void()> test,
                  const std::source_location location) {
        TestName name(group_name, test_name, location);
        max_group_width = std::max(max_group_width, static_cast<int>(group_name.size()));
        max_test_width = std::max(max_test_width, static_cast<int>(test_name.size()));
        if (multithreaded) {
            if (mt_tests.contains(name)) {
                throw std::runtime_error("Multithreaded tests already contains " + group_name +
                                         ":" + test_name);
            }
            mt_tests[name] = std::move(test);
        } else {
            if (st_tests.contains(name)) {
                throw std::runtime_error("Singlethreaded tests already contains " + group_name +
                                         ":" + test_name);
            }
            st_tests[name] = std::move(test);
        }
    }

    void run_all_tests() {
        const int total_tests = static_cast<int>(mt_tests.size() + st_tests.size());
        auto start = std::chrono::high_resolution_clock::now();
        failed_tests.clear();
        run_all_mt_tests();
        run_all_st_tests();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = duration_cast<std::chrono::microseconds>(stop - start);
        print_results(failed_tests, duration, total_tests, max_group_width, max_test_width);
    }

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

    void run_all_st_tests() {
        for (const auto &[name, test]: st_tests) {
            print_running_test(name, max_group_width, max_test_width);
            try {
                auto start = std::chrono::high_resolution_clock::now();
                test();
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = duration_cast<std::chrono::microseconds>(stop - start);
                print_test_passed(name, duration, max_group_width, max_test_width);
            } catch (const std::exception &e) {
                print_test_failed(name, e, max_group_width, max_test_width);
                failed_tests.push_back(name);
            }
        }
    }

    void run_all_mt_tests() {
        std::counting_semaphore<> semaphore(max_concurrent_threads);
        std::vector<std::future<void>> futures;
        std::mutex output_mutex; // Mutex for synchronized output

        int group_width = max_group_width;
        int test_width = max_test_width;
        std::vector<TestName> mt_failed_tests;
        for (const auto &[name, test]: mt_tests) {
            semaphore.acquire();
            futures.push_back(
                std::async(std::launch::async, [&name, &test, &semaphore, &output_mutex,
                                                &group_width, &test_width, &mt_failed_tests]() {
                    try {
                        {
                            std::lock_guard<std::mutex> lock(output_mutex);
                            print_running_test(name, group_width, test_width);
                        }
                        auto start = std::chrono::high_resolution_clock::now();
                        test();
                        auto stop = std::chrono::high_resolution_clock::now();
                        auto duration = duration_cast<std::chrono::microseconds>(stop - start);
                        {
                            std::lock_guard<std::mutex> lock(output_mutex);
                            print_test_passed(name, duration, group_width, test_width);
                        }
                    } catch (const std::exception &e) {
                        std::lock_guard<std::mutex> lock(output_mutex);
                        print_test_failed(name, e, group_width, test_width);
                        mt_failed_tests.push_back(name);
                    }
                    semaphore.release();
                }));
        }

        for (auto &future: futures) {
            future.wait();
        }
        failed_tests.insert(failed_tests.end(), mt_failed_tests.begin(), mt_failed_tests.end());
    }
};
