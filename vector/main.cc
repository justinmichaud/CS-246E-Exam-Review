#include <iostream>
#include <vector>

template<typename T>
struct MyAllocator {
    using value_type = T;
    template<typename U> struct rebind {
        using other = MyAllocator<U>;
    };

    T *allocate(size_t n) {
        std::cout << "Allocate " << n << "\n";
        return static_cast<T*>(malloc(sizeof(T)*n));
    }

    void deallocate(T *p, size_t n) {
        free(p);
        std::cout << "Deallocate " << p << " of size " << n << "\n";
    }
};

class C {
public:
    int a=0;
    C(int a): a{a} {}
};

int main() {
    std::vector<C, MyAllocator<C>> v;
    v.push_back(C{5});

    std::cout << v[0].a << '\n';
}