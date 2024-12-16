
module;
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
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
export template <typename T, typename U>
void assert_eq(const T &expected, const U &actual, const std::string &message = "") {
    if (!(expected == actual)) { // Allow implicit comparison
        // Use stringstreams for type-safe conversion to string
        std::ostringstream oss;
        if (message.empty()) {
            oss << "Assertion failed: expected " << expected << ", got " << actual;
        } else {
            oss << message;
        }
        throw std::runtime_error(oss.str());
    }
}

export template <typename T, typename U>
void assert_ne(const T &expected, const U &actual, const std::string &message = "") {
    if (expected == actual) { // Allow implicit comparison
        // Use stringstreams for type-safe conversion to string
        std::ostringstream oss;
        if (message.empty()) {
            oss << "Assertion failed: expected " << expected << ", got " << actual;
        } else {
            oss << message;
        }
        throw std::runtime_error(oss.str());
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