//g++ main.cc -o main -Wall -std=c++14 -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -g && ./main
#include <iostream>
#include <vector>
#include <cstring>

template<typename T>
void uninitialized_move_or_copy(T *to, T *from, T *end) {
    while (from != end) {
        if (std::is_nothrow_move_constructible<T>::value) {
            new (to) T{std::move(*from)};
        } else {
            new (to) T{*from};
        }
        ++from; ++to;
    }
}

template<typename T>
void uninitialized_copy(std::enable_if_t<!std::is_pod<T>::value, T> *to, const T *from, const T *end) {
    while (from != end) {
        new (to) T{*from};
        ++from; ++to;
    }
    std::cout << "uninit copy non pod called\n";
}

template<typename T>
void uninitialized_copy(std::enable_if_t<std::is_pod<T>::value, T> *to, const T *from, const T *end) {
    memcpy(static_cast<void *>(to), 
        static_cast<const void *>(from), 
        sizeof(T) * (end-from));
    std::cout << "uninit copy pod called\n";
}

template<typename T, typename Alloc = std::allocator<T>>
class vector {
    struct vector_base: Alloc {
        size_t cap;
        size_t n;
        T* arr;

        using Alloc::allocate;
        using Alloc::deallocate;

        vector_base(size_t cap): cap{cap == 0? 1: cap}, n{0},
            arr{allocate(this->cap)} {}
        vector_base(const vector_base &o): vector_base{o.cap} {
            uninitialized_copy(arr, o.arr, o.arr+o.n);
            n = o.n;
        }
        vector_base(vector_base &&o): cap{o.cap}, n{o.n}, arr{o.arr} { o.cap = 0; o.n = 0; o.arr = nullptr; }
        ~vector_base() { for (size_t i=0; i<n; ++i) arr[i].~T(); n = 0; deallocate(arr, cap); cap = 0; }
        vector_base &operator=(vector_base o) { std::swap(cap, o.cap); std::swap(n, o.n); std::swap(arr, o.arr); return *this; }
    };

    vector_base vb = vector_base{0};

    void increaseCap() {
        if (vb.n == vb.cap) {
            vector_base vb2{vb.cap*2};
            vb2.n = vb.n;
            uninitialized_move_or_copy(vb2.arr, vb.arr, vb.arr+vb.n);
            std::swap(vb, vb2);
        }
    }
public:

    void push_back(T t) {
        increaseCap();
        new (vb.arr + (vb.n++)) T{t};
    }

    void pop_back() {
        vb.arr[--vb.n].~T();
    }

    T &operator[](size_t i) {
        return vb.arr[i];
    }
};

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
    std::string *s;
    C(int a): a{a}, s{new std::string{"hello"}} {}
    C(const C &o): a{o.a}, s{new std::string{*o.s}} {}
    ~C() { delete s; }
};

int main() {
    vector<C, MyAllocator<C>> v;
    v.push_back(C{5});
    v.push_back(C{7});
    v.push_back(C{8});
    vector<C, MyAllocator<C>> v2 = v;
    v.pop_back();
    std::cout << v[0].a << '\n';

    vector<int, MyAllocator<int>> v3;
    v3.push_back(5);
    v3.push_back(7);
    vector<int, MyAllocator<int>> v4 = v3;
    v3.pop_back();
    std::cout << v4[0] << std::endl;
}
