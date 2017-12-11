#include <iostream>
#include <cassert>

template<typename T> T myforward(std::remove_reference_t<T>& arg) {
    return static_cast<T>(arg);
    std::cout << "lvalue forward\n";
}

template<typename T> T myforward(std::remove_reference_t<T>&& arg) {
    return static_cast<T>(arg);
    std::cout << "rvalue forward\n";
}

void test(int &x) {
    std::cout << x << " is an lvalue\n";
}

void test(int &&x) {
    std::cout << x << " is an rvalue\n";
}

int main() {
    int a = 0;
    int b = 1;
    test(myforward<int&>(a));
    test(myforward<int&&>(std::move(b)));
    test(myforward<int&&>(a));
    test(myforward<int&>(std::move(b)));
    test(myforward<int&&>(5));
}