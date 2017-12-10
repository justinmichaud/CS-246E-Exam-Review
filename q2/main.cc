#include <iostream>

template <typename T>
class C {
public:
    void doSomethingOld(T& copy) {
        std::cout << "Copy\n";
    }

    void doSomethingOld(T&& move) {
        std::cout << "move\n";
    }

    template<typename U>
    void doSomething(U&& unified) {
        std::cout << "unified\n";
        doSomethingOld(std::forward<U>(unified));

    }
};

class D {};

int main() {
    D d1;
    C<D> c;

    c.doSomethingOld(d1);
    c.doSomethingOld(D{});
    c.doSomething(d1);
    c.doSomething(D{});
}