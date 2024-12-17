import std;
import CPPUnitTest;

const static TestRegister test_addition("PerformanceTest", "AdditionTest", true, [] {
    int a = 2;
    int b = 2;
    assert_eq(4, a + b, "Addition test failed");
});

const static TestRegister test_division("PerformanceTest", "DivisionTest", true, [] {
    int a = 4;
    int b = 2;
    assert_eq(2, a / b, "Division test failed");
    assert_ne(2, a / b, "Division test failed");
    assert_throw<std::runtime_error>([] { throw std::runtime_error("Error"); },
                                     "Exception not thrown as expected");
});

const static TestRegister test_substraction("PerformanceTest", "SubtractionTest", true, [] {
    int a = 4;
    int b = 2;
    assert_eq(2, a - b, "Subtaction test failed");
});
