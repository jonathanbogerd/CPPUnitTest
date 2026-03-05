module;
#include <stdexcept>
#include <string>
module CPPUnitTest;

import :Assert;
void assert_true(bool condition, const std::string &message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void assert_false(bool condition, const std::string &message) {
    if (condition) {
        throw std::runtime_error(message);
    }
}