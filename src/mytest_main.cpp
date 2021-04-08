#include "mytest.h"

int main() {
    mytest::MytestRunner::start();
    if (context::get_failure_counter() > 0) {
        return 1;
    }
    return 0;
}

