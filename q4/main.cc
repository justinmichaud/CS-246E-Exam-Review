#include <iostream>

template <typename Base>
class Assignable {
public:
    Base& operator=(Base other) {
        static_cast<Base*>(this)->swap(other);
        return *static_cast<Base*>(this);
    }
};

class C: private Assignable<C> {
public:
    int a = 0;
    C(int a): a{a} {}

    using Assignable<C>::operator=;

    void swap(C &other) {
        std::cout << "Called swap\n";
        std::swap(a, other.a);
    }
};

int main() {
    C c {5};
    C c2 {6};

    c2 = c;
    std::cout << c2.a << std::endl;
}