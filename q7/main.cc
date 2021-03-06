#include <iostream>
#include <cassert>

template<typename T> T&& myforward(std::remove_reference_t<T>& arg) {
    return static_cast<T&&>(arg);
    std::cout << "lvalue forward\n";
}

template<typename T> T&& myforward(std::remove_reference_t<T>&& arg) {
    return static_cast<T&&>(arg);
    std::cout << "rvalue forward\n";
}

template <typename T>
class m_unique_ptr {
    T *data;
public:
    m_unique_ptr(): data{nullptr} {}
    m_unique_ptr(T *data): data{data} {}
    m_unique_ptr(const m_unique_ptr<T> &other) = delete;
    m_unique_ptr(m_unique_ptr<T> &&other): data{other.data} { other.data = nullptr; }
    ~m_unique_ptr() { delete data; }

    m_unique_ptr<T> operator=(const m_unique_ptr<T> &other) = delete;
    m_unique_ptr<T> operator=(m_unique_ptr<T> &&other) { std::swap(data, other.data); }

    T &operator*() { return *data; }
    T &operator[](size_t i) { return *(data + i); }

    T *get() { return data; }
};

template <typename T>
class m_unique_ptr<T[]> {
    T *data;
public:
    m_unique_ptr(): data{nullptr} {}
    m_unique_ptr(T *data): data{data} {}
    m_unique_ptr(const m_unique_ptr<T[]> &other) = delete;
    m_unique_ptr(m_unique_ptr<T[]> &&other): data{other.data} { other.data = nullptr; }
    ~m_unique_ptr() { delete[] data; }

    m_unique_ptr<T[]> operator=(const m_unique_ptr<T[]> &other) = delete;
    m_unique_ptr<T[]> operator=(m_unique_ptr<T[]> &&other) { std::swap(data, other.data); }

    T &operator*() { return *data; }
    T &operator[](size_t i) { return *(data + i); }

    T *get() { return data; }
};

template <typename T, typename... Args>
std::enable_if_t<!std::is_same<std::remove_extent_t<T>, T>::value, m_unique_ptr<T>>
m_make_unique(Args&... args) {
    return m_unique_ptr<T> {new std::remove_extent_t<T>[sizeof...(args)] {myforward<Args>(args)...}};
}

template <typename T, typename... Args>
std::enable_if_t<std::is_same<std::remove_extent_t<T>, T>::value, m_unique_ptr<T>>
m_make_unique(Args&... args) {
    return m_unique_ptr<T> {new T {myforward<Args>(args)...}};
}

int main() {
    using namespace std;
    m_unique_ptr<string> ptr1 {new string{"Hello world!"}};
    cout << *ptr1 << '\n';

    m_unique_ptr<string> ptr2 {std::move(ptr1)};
    cout << *ptr2 << '\n';
    assert(ptr1.get() == nullptr);

    m_unique_ptr<string> ptr3 = m_make_unique<string>("hello.");
    cout << *ptr3 << '\n';

    m_unique_ptr<string[]> ptr4 {new string[2] {"hello.", "world!"}};
    cout << ptr4[0] << " --- " << ptr4[1] << "\n";

    m_unique_ptr<string[]> ptr5 = m_make_unique<string[]>("hello.", "world!");
    cout << ptr5[0] << " --- " << ptr5[1] << "\n";
}