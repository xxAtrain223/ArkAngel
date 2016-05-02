#include <raspberry/raspberry.hpp>
#include <iostream>

RASPBERRY_DECL_METHOD(SpeakConcept, speak);

using Animal = raspberry::Any<SpeakConcept<void()>>;

struct Cat {
    void speak() {
        std::cout << "Meow!" << std::endl;
    }
};

int main(int argc, char** argv) {
    Animal animal = Cat{};
    animal.speak();
}
