#include <iostream>

// pretty print library
#include <fmt/core.h>

int main(int argc, char* argv[]) {
    using std::cout;
    using std::endl;
    cout << "Hello World!!!" << endl;
    fmt::print("Hello World from {} using {}", "fmt", "conan");
    return 0;
}