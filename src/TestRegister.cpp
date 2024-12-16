module;
#include <functional>
#include <string>
export module CPPUnitTest:TestRegister;

import :TestManager;

export class TestRegister {
  public:
    TestRegister(const std::string &group_name, const std::string &test_name,
                 const bool multithreaded, std::function<void()> test) {
        TestManager::instance().add_test(group_name, test_name, multithreaded, std::move(test));
    }
};