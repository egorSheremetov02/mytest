#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include "mytest_context.h"
#include "mytest_fwd.h"

#ifndef MYTEST_H_
#define MYTEST_H_

namespace mytest {
    struct Test {
    private:
        std::string name;
        std::function<void()> callback;

    public:
        Test(std::string _name, std::function<void()> _callback);

        void run() const;

        [[nodiscard]] std::string const &get_name() const;
    };

    struct MytestHolder {
    private:
        std::deque<Test> tests;

    public:
        void add_test(Test const &test);

        [[nodiscard]] int amount_of_tests() const;

        [[nodiscard]] std::deque<Test> const &get_tests();
    };

    struct MytestRunner {
        static void start();
    };

    void log_start_test(Test const &test);

    void log_end_of_tests();

    void log_error(std::string const &filename,
                   int line,
                   std::string const &expression);

    void log_error_with_msg(std::string const &filename,
                            int line,
                            std::string const &expression,
                            std::string const &msg);

    void register_test(std::string const &name,
                       std::function<void()> const &callback);

// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define MYTEST_INTERNAL_TEST_CASE(class_name, filename, name, line) \
    static void class_name();                                       \
    namespace {                                                     \
    struct InitTest##line {                                         \
        InitTest##line() {                                          \
            mytest::register_test(name, class_name);                \
        }                                                           \
    } init_testcase_##line;                                         \
    }                                                               \
    void class_name()

// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define MYTEST_INTERNAL_TEST_CASE2(line, filename, name) \
    MYTEST_INTERNAL_TEST_CASE(mytest_test_class_##line, filename, name, line)
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define MYTEST_INTERNAL_TEST_CASE1(line, filename, name) \
    MYTEST_INTERNAL_TEST_CASE2(line, filename, name)
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define TEST_CASE(name) MYTEST_INTERNAL_TEST_CASE1(__LINE__, __FILE__, name)

// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define MYTEST_INTERNAL_CHECK(line, file, expr)   \
    [&]() {                                       \
        if (!(expr)) {                            \
            context::is_failed() = true;          \
            mytest::log_error(file, line, #expr); \
        }                                         \
    }()

// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define CHECK(expr) MYTEST_INTERNAL_CHECK(__LINE__, __FILE__, expr)

// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define MYTEST_INTERNAL_CHECK_WITH_MSG(line, file, expr, msg)   \
    [&]() {                                                     \
        if (!(expr)) {                                          \
            context::is_failed() = true;                        \
            mytest::log_error_with_msg(file, line, #expr, msg); \
        }                                                       \
    }()
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define CHECK_MESSAGE(expr, msg) \
    MYTEST_INTERNAL_CHECK_WITH_MSG(__LINE__, __FILE__, expr, msg)
}  // namespace mytest

#endif  // MYTEST_H_