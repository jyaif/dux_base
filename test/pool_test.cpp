#include <cassert>

#include "pool.h"

void TestPool() {
    dux::Pool<int8_t> pool;
    for (int i = 0; i < 100; i++) {
        pool.GetNewPtr();
    }
    pool.Reset();
    // Now, at least the first 100 pointers must be back to back.
    int8_t* previous = pool.GetNewPtr();
    for (int i = 0; i < 99; i++) {
        int8_t* current = pool.GetNewPtr();
        assert(current - previous == 1);
        previous = current;
    }

    int8_t* ptr = pool.New(42);
    assert(*ptr == 42);
}