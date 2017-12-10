#include<iostream>
#include<cassert>
#include<iomanip>

template <typename... Args> using ignore_args = void;

template<typename, typename T>
struct is_default_constructible_helper {
    const static bool value = false;
};

template<typename T>
struct is_default_constructible_helper<std::enable_if_t<std::is_same<decltype(T{}), T>::value, ignore_args<>>, T> {
    const static bool value = true;
};

template<typename T>
struct is_default_constructible: is_default_constructible_helper<ignore_args<>, T> {};

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