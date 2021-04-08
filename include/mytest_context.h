#include "mytest_fwd.h"

#ifndef MYTEST_CONTEXT_H
#define MYTEST_CONTEXT_H
namespace context {
    mytest::MytestHolder &get_holder();

    int &get_failure_counter();

    bool &is_failed();
}  // namespace context

#endif  // MYTEST_CONTEXT_H