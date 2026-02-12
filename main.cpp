#include <iostream>
#include "util.h"
#include "pointers.h"
#include "containers/array.h"
#include "containers/lists.h"

void DemoQueue(); 

// g++ -std=c++17 -Wall -g -pthread main.cpp DemoStack.cpp DemoQueue.cpp -o main

int main() {
    std::cout << "       EDA - UNMSM 2026           " << std::endl;

    // --- Demos Anteriores ---
    // DemoUtil();
    // DemoPointers1();
    // DemoArray();
    // DemoLists();

   
    DemoQueue();   


}