#include <string>
import CPPUnitTest;

const static TestRegister test_string_equals("StringTest", "StringEquals", true, [] {
    std::string a = "TestString";
    assert_eq(a, "TestString", "String equals test failed");
});

const static TestRegister test_string_not_equals("StringTest", "StringNotEquals", true, [] {
    std::string a = "DifferentString";
    assert_ne(a, "TestString", "String not equals test failed");
});