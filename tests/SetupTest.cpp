import std;
import CPPUnitTest;
constexpr int a = 2;
constexpr int b = 2;
const static TestRegister test_addition("PerformanceTest", "AdditionTest", true, [a = a, b = b] {
    assert_eq(4, a + b, "Addition test failed");
});

const static TestRegister test_division("PerformanceTest", "DivisionTest", true, [a = a, b = b] {
    assert_eq(1, a / b, "Division test failed");
    assert_ne(1, a / b, "Division test failed");
    assert_throw<std::runtime_error>([] { throw std::runtime_error("Error"); },
                                     "Exception not thrown as expected");
});

const static TestRegister test_substraction("PerformanceTest", "SubtractionTest", true,
                                            [a = a, b = b] {
                                                assert_eq(0, a - b, "Subtaction test failed");
                                            });
