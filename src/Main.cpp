#include "Engine.cpp"
#include <iostream>

int main() try {
    Engine{}.go();
    return EXIT_SUCCESS;
} catch (const std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return EXIT_FAILURE;
}
