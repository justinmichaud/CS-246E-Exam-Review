#include <iostream>
#include <cassert>

template <typename T>
T create_T(std::enable_if_t<std::is_default_constructible<T>::value,int>) {
    return T{};
}

template <typename T>
T create_T(std::enable_if_t<!std::is_default_constructible<T>::value,int>) {
    return T{1};
}

template <typename T>
T create_T() {
    return create_T<T>(5);
}

struct C { C(int a) { assert(a == 1); } };

int main() {
    using namespace std;
    int i = create_T<int>();
    C c = create_T<C>();
}