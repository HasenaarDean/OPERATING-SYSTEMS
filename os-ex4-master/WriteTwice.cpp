#include "VirtualMemory.h"

#include <cstdio>
#include <cassert>

int main(int argc, char **argv) {
    VMinitialize();
    VMwrite(5 * PAGE_SIZE, 30);
    VMwrite(5 * PAGE_SIZE, 30);
    word_t value = 0;
    VMread(5 * PAGE_SIZE, &value);
    assert(uint64_t(value) == 30);
    return 0;
}