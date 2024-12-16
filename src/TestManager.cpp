module;
#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <limits>
#include <map>
#include <string>
export module CPPUnitTest:TestManager;

struct TestName {
    std::string group_name;
    std::string test_name;

    bool operator<(const TestName &other) const {
        // Compare by group_name first, then by test_name
        return (group_name < other.group_name) ||
               (group_name == other.group_name && test_name < other.test_name);
    }
};

void print_running_test(const TestName &name) {
    std::cout << "Running test: " << name.group_name << " :" << name.test_name << '\n';
}

void print_test_passed(const TestName &name, const std::chrono::microseconds &duration) {
    std::cout << "Test passed: " << name.group_name << " :" << name.test_name << " in "
              << duration.count() << " microseconds \n";
}

void print_test_failed(const TestName &name, const std::exception &exc) {
    std::cerr << "Test failed: " << name.group_name << " :" << name.test_name << " - " << exc.what()
              << "\n";
}

// TestManager: Singleton to manage test registration and execution
export class TestManager {
  public:
    static TestManager &instance() {
        static TestManager manager;
        return manager;
    }

    void add_test(const std::string &group_name, const std::string &test_name,
                  const bool multithreaded, std::function<void()> test) {
        TestName name(group_name, test_name);
        if (multithreaded) {
            mt_tests[name] = std::move(test);
        } else {
            st_tests[name] = std::move(test);
        }
    }

    void run_all_tests() const {
        run_all_mt_tests();
        run_all_st_tests();
    }

    void run_all_st_tests() const {
        for (const auto &[name, test]: st_tests) {
            print_running_test(name);
            try {
                auto start = std::chrono::high_resolution_clock::now();
                test();
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = duration_cast<std::chrono::microseconds>(stop - start);
                print_test_passed(name, duration);
            } catch (const std::exception &e) {
                print_test_failed(name, e);
            }
        }
    }

    // Function to run all multithreaded tests with limited concurrency and synchronized output
    void run_all_mt_tests() const {
        std::counting_semaphore<> semaphore(max_concurrent_threads);
        std::vector<std::future<void>> futures;
        std::mutex output_mutex; // Mutex for synchronized output

        for (const auto &[name, test]: mt_tests) {
            semaphore.acquire();
            futures.push_back(
                std::async(std::launch::async, [&name, &test, &semaphore, &output_mutex]() {
                    try {
                        {
                            std::lock_guard<std::mutex> lock(output_mutex);
                            print_running_test(name);
                        }
                        auto start = std::chrono::high_resolution_clock::now();
                        test();
                        auto stop = std::chrono::high_resolution_clock::now();
                        auto duration = duration_cast<std::chrono::microseconds>(stop - start);
                        {
                            std::lock_guard<std::mutex> lock(output_mutex);
                            print_test_passed(name, duration);
                        }
                    } catch (const std::exception &e) {
                        std::lock_guard<std::mutex> lock(output_mutex);
                        print_test_failed(name, e);
                    }
                    semaphore.release();
                }));
        }

        for (auto &future: futures) {
            future.wait();
        }
    }

    TestManager() = default;
    ~TestManager() = default;

    TestManager(const TestManager &) = delete;
    TestManager &operator=(const TestManager &) = delete;

  private:
    std::map<TestName, std::function<void()>> st_tests;
    std::map<TestName, std::function<void()>> mt_tests;
#if CPPUNITTEST_MAX_THREADS
    int max_concurrent_threads = CPPUNITTEST_MAX_THREADS;
#else
    int max_concurrent_threads = std::numeric_limits<int>::max();
#endif
};
