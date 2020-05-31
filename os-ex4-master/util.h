//
// Created by imri on 5/22/2019.
//

#ifndef OS_EX4_UTIL_H
#define OS_EX4_UTIL_H

#ifdef DEBUG
#include <cstdio>
#define PRINT(x, y) printf(x, y)
#define PRINT_2(x, y, z) printf(x, y, z)
#else
#define PRINT(x, y)
#define PRINT_2(x, y, z) printf(x, y, z)
#endif

#include <cstdint>
#include <algorithm>
#include "MemoryConstants.h"
#include "PhysicalMemory.h"
#include <cassert>

uint64_t translateAddress(uint64_t virtualAddress);

void clearTable(uint64_t frameIndex);

#endif //OS_EX4_UTIL_H
