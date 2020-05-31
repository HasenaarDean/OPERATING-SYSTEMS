#include "VirtualMemory.h"

#include <cstdio>
#include <cassert>
#include "util.h"

int main(int argc, char **argv) {
    assert("100" == binaryRepresentation(4));
    assert("10100110001001101101011" == binaryRepresentation(5444459));
}