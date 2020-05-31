#include "VirtualMemory.h"

#include <cstdio>
#include <cassert>

uint64_t address(uint64_t i){
    return 5 * i * PAGE_SIZE;
}

int main(int argc, char **argv) {
    VMinitialize();
    for (uint64_t i = 0; address(i) < VIRTUAL_MEMORY_SIZE; ++i) {
        printf("writing to %llu\n", (long long int) address(i));
        VMwrite(address(i), i);
    }

    for (uint64_t i = 0; address(i) < VIRTUAL_MEMORY_SIZE; ++i) {
        word_t value;
        printf("reading from %llu %d\n", (long long int) address(i), value);
        VMread(address(i), &value);
        assert(uint64_t(value) == i);
    }
    printf("success\n");

    return 0;
}