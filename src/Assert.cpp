
module;
#include <stdexcept>
#include <string>
export module CPPUnitTest:Assert;

// Assertion functions
export void assert_true(bool condition,
                        const std::string &message = "Assertion failed: expected true") {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

export void assert_false(bool condition,
                         const std::string &message = "Assertion failed: expected false") {
    if (condition) {
        throw std::runtime_error(message);
    }
}

export template <typename T>
void assert_eq(const T &expected, const T &actual, const std::string &message = "") {
    if (expected != actual) {
        throw std::runtime_error(message.empty()
                                     ? "Assertion failed: expected " + std::to_string(expected) +
                                           ", got " + std::to_string(actual)
                                     : message);
    }
}

export template <typename T>
void assert_ne(const T &expected, const T &actual, const std::string &message = "") {
    if (expected == actual) {
        throw std::runtime_error(message.empty()
                                     ? "Assertion failed: expected not " +
                                           std::to_string(expected) + ", but both values are equal"
                                     : message);
    }
}

export template <typename ExceptionType, typename Callable>
void assert_throw(Callable callable, const std::string &message = "") {
    bool caught = false;
    try {
        callable();
    } catch (const ExceptionType &) {
        caught = true;
    }
    if (!caught) {
        throw std::runtime_error(message.empty() ? "Assertion failed: exception not thrown"
                                                 : message);
    }
}