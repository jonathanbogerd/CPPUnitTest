module;

export module CPPUnitTest;

export import :TestManager;
export import :Assert;
export import :TestRegister;

import :TestManager;

export extern "C++" int main() {

    TestManager::instance().run_all_tests();
    return 0;
}