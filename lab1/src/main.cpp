#include <iostream>

#include "SFML/window.h"

int main() {
    try {
        Window window("Lab1", 500, 500, 60);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}