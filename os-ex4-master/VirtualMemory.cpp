#include "VirtualMemory.h"
#include "PhysicalMemory.h"
#include "util.h"

void VMinitialize() {
    clearTable(0);
}


int VMread(uint64_t virtualAddress, word_t *value) {
    if (virtualAddress > VIRTUAL_MEMORY_SIZE) return 0;
    uint64_t physicalAddress = translateAddress(virtualAddress);
    PMread(physicalAddress, value);
    return 1;
}


int VMwrite(uint64_t virtualAddress, word_t value) {
    if (virtualAddress > VIRTUAL_MEMORY_SIZE) return 0;
    uint64_t physicalAddress = translateAddress(virtualAddress);
    PMwrite(physicalAddress, value);
    return 1;
}
