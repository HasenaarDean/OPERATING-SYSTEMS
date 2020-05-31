#include <iostream>
#include "osm.h"

using namespace std;


int main() {
    cout << "operation: " << osm_operation_time(1000000) << endl;
    cout << "function call: " << osm_function_time(1000000) << endl;
    cout << "system call: " << osm_syscall_time(1000000) << endl;
    return 0;
}