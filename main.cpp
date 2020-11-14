#include <iostream>
#include "demo.h"

int main (int argc, char * const argv[]) {
    // insert code here...
    std::cout << "Start\n";
    
    koen::CDemo *demo = new koen::CDemo();
    demo->run();
    delete demo;
    
    std::cout << "End\n";
    return 0;
}
