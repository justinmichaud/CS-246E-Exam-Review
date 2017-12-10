#include <iostream>
#include <vector>

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
void uninitialized_copy(T *to, const T *from, const T *end) {
    while (from != end) {
        new (to) T{*from};
        ++from; ++to;
    }
    std::cout << "uninit copy called\n";
}

// TODO memcpy if pod

template<typename T, typename Alloc = std::allocator<T>>
class vector {
    struct vector_base: Alloc {
        size_t cap;
        T* arr;

        using Alloc::allocate;
        using Alloc::deallocate;

        vector_base(size_t cap): cap{cap == 0? 1: cap}, 
            arr{allocate(this->cap)} {}
        vector_base(const vector_base &o): vector_base{o.cap} {
            uninitialized_copy(arr, o.arr, o.arr+o.cap);
        }
        vector_base(vector_base &&o): cap{o.cap}, arr{o.arr} { o.cap = 0; o.arr = nullptr; }
        ~vector_base() { deallocate(arr, cap); cap = 0; }
        vector_base &operator=(vector_base o) { std::swap(cap, o.cap); std::swap(arr, o.arr); return *this; }
    };

    vector_base vb;
    size_t n;

    void increaseCap() {
        if (n == vb.cap) {
            vector_base vb2{vb.cap == 0? 1 : vb.cap*2};
            uninitialized_move_or_copy(vb2.arr, vb.arr, vb.arr+vb.cap);
            std::swap(vb, vb2);
        }
    }
public:
    vector(): vb{0}, n{0} {}
    vector(const vector& o): vb{o.vb}, n{o.n} {}
    vector(vector &&o): vb{std::move(o.vb)}, n{o.n} { o.n = 0; }
    vector<T> &operator=(vector o) { std::swap(vb, o.vb); std::swap(n, o.n); return *this; }

    void push_back(T t) {
        increaseCap();
        new (vb.arr + (n++)) T{t};
    }

    void pop_back() {
        vb.arr[--n].~T();
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
    C(int a): a{a} {}
};

int main() {
    vector<C, MyAllocator<C>> v;
    v.push_back(C{5});
    v.push_back(C{7});
    v.pop_back();

    vector<C, MyAllocator<C>> v2 = v;

    std::cout << v[0].a << '\n';
}