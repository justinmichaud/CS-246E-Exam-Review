#include <memory>
#include <iostream>

template<typename T>
struct remove_reference {
    using type = T;
};

template<typename T>
struct remove_reference<T&> {
    using type = T;
};

template<typename T>
struct remove_reference<T&&> {
    using type = T;
};

template <typename T> using remove_reference_t = typename remove_reference<T>::type;

template<typename T>
remove_reference_t<T>&& mymove(T&& o) {
    return static_cast<remove_reference_t<T>&&>(o);
}

struct C {
    int x = 0;
    C() {}
    C(C&& other): x{other.x} {}
};

int main() {
    using namespace std;
    C c1;
    C c2 {mymove(c1)};
}
