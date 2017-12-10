#include <memory>
#include <iostream>

class C {
    static void * mem;
    static bool allocated;

public:
    int a=0;

    void *operator new(size_t n) {
        if (n == 0) n = sizeof(C);
        if (n!=sizeof(C) || allocated) throw std::bad_alloc{};
        allocated = true;
        return mem;
    }

    void operator delete(void *p) noexcept {
        if (mem == p) allocated = false;
    }
};

void *C::mem = malloc(sizeof(C)); 
bool C::allocated = false;

int main() {
    using namespace std;
    // C c1;
    // c1.a = 5;

    C *c2 = new C;
    c2->a = 7;

    std::cout << c2->a << '\n';

    try {
        C *c3 = new C;
    } catch (std::bad_alloc &e) {
        cout << "caught bad alloc\n";
    }

    delete c2;
    C *c4 = new C;

    // cout << c1.a << '\n';
    cout << c4->a << '\n';
    cout << c2->a << '\n';

    c4->a = 99;
    cout << c4->a << '\n';
    cout << c2->a << '\n';
}
