#ifndef INCLUDED_TESTER_INL
#define INCLUDED_TESTER_INL 1


#include <cstddef>
#include <iostream>


namespace Tester {

static std::size_t errors = 0;

#define test_assert(expression) \
    ((expression) ? (void) 0 : ::Tester::test_assert_fail(__FILE__,__LINE__,__func__,#expression))

static void test_assert_fail(
        const char *fileName,
        std::size_t lineNumber,
        const char *functionName,
        const char *expression) {
    errors++;
    std::cerr << fileName << ":" << lineNumber <<
        ": Assertion failure in function \"" << functionName <<
        "\": " << expression << std::endl;
}

} // namespace Tester

#endif // #ifndef INCLUDED_TESTER_INL

/* vim: set et sw=4 sts=4 tw=79: */
