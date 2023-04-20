#include <cstdio>
#include <string>
#include <sstream>

std::string sizet_to_string(size_t value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

int main() {
    size_t a = 121212121212;
    std::string b = sizet_to_string(a);
    std::printf("b : %s", b.c_str());
    return 0;
}