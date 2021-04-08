#include "mytest.h"
#include <functional>
#include <string>
#include "mytest_context.h"

namespace context {
    mytest::MytestHolder &get_holder() {
        static mytest::MytestHolder holder;
        return holder;
    }

    bool &is_failed() {
        static bool failed = false;
        return failed;
    }

    int &get_failure_counter() {
        static int counter = 0;
        return counter;
    }
}  // namespace context

namespace mytest {
    namespace {
// File-local declarations and definitions.
    }  // namespace

// TEST implementation
    Test::Test(std::string _name, std::function<void()> _callback)
            : name(std::move(_name)), callback(std::move(_callback)) {
    }

    void Test::run() const {
        callback();
    }

    std::string const &Test::get_name() const {
        return name;
    }

// MYTEST_REGISTRY implementation
    void register_test(std::string const &name,
                       std::function<void()> const &callback) {
        Test test(name, callback);
        context::get_holder().add_test(test);
    }

// MYTEST_HOLDER implementation
    void MytestHolder::add_test(const Test &test) {
        tests.push_back(test);
    }

    int MytestHolder::amount_of_tests() const {
        return static_cast<int>(tests.size());
    }

    std::deque<Test> const &MytestHolder::get_tests() {
        return tests;
    }

// MYTEST_RUNNER implementation
    void MytestRunner::start() {
        std::deque<Test> const &test_queue = context::get_holder().get_tests();
        for (Test const &test : test_queue) {
            log_start_test(test);
            test.run();
            if (context::is_failed()) {
                context::get_failure_counter()++;
            }
            context::is_failed() = false;
        }
        log_end_of_tests();
    }

// MYTEST_LOGGER
    void log_start_test(const Test &test) {
        std::cerr << "Running "
                  << "\"" << test.get_name() << "\""
                  << "..." << '\n';
    }

    void log_end_of_tests() {
        int test_size = context::get_holder().amount_of_tests();
        std::cerr << "===== "
                  << "Tests passed: "
                  << (test_size - context::get_failure_counter()) << "/"
                  << test_size << " =====" << '\n';
    }

    void log_error(std::string const &filename, int line, std::string const &expr) {
        std::cerr << "CHECK(" << expr << ") at " << filename << ":" << line
                  << " failed!" << '\n';
    }

    void log_error_with_msg(std::string const &filename,
                            int line,
                            std::string const &expression,
                            std::string const &msg) {
        log_error(filename, line, expression);
        std::cerr << "    "
                  << "message: " << msg << '\n';
    }

// Externally visible declarations and definitions.
}  // namespace mytest