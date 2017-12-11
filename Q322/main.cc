#include<iostream>
#include<cassert>
#include<iomanip>

template<typename T, typename>
struct is_default_constructible_helper {
    const static bool value = false;
};

template<typename T>
struct is_default_constructible_helper<decltype(T{}), T> {
    const static bool value = true;
};

template<typename T>
struct is_default_constructible: is_default_constructible_helper<T, T> {};

class N{
public:
    N(int i){}
};

int main(){
    std::cout<<std::boolalpha;
    std::cout<<is_default_constructible<int>::value<<std::endl;
    std::cout<<is_default_constructible<N>::value<<std::endl;
    return 0;
}