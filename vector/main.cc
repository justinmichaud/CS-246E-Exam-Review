#include <iostream>
#include <vector>

class C {
public:
    int a=0;
    C(int a): a{a} {}
};

int main() {
    std::vector<C> v;
    v.push_back(C{5});

    std::cout << v[0].a << '\n';
}