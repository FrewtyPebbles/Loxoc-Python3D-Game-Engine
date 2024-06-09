#pragma once
#include <stdexcept>
#include <iostream>


template<typename T, size_t N>
class tup {
public:
    tup() : data{}, size(N) {}
    tup(T values[N]) {
        memcpy(data, values, sizeof(T)*N);
    }
    tup(const tup& o) = default;
    
    T& operator[](size_t index) {
        if (index >= N) {
            throw std::out_of_range("Index out of range.");
        }
        return this->data[index];
    }

    T data[N];
    size_t size = N;
    friend inline std::ostream& operator<<(std::ostream& os, const tup<T, N>& self){
        os << '(';
        for (int i = 0; i < N; i++)
            os << (T)self.data[i] << ',';
        os << ')';
        return os;
    }
    inline constexpr auto* begin() {
        return std::cbegin(this->data);
    }

    inline constexpr auto* end() {
        return std::cend(this->data);
    }
};


template <typename T, size_t N>
inline tup<T, N> make_tup(std::initializer_list<T> values) {
    if (values.size() != N) {
        throw std::invalid_argument("Inputs do not match tuple size.");
    }
    auto ret = tup<T, N>();
    std::copy(values.begin(), values.end(), ret.data);
    return ret;
}


typedef tup<tup<float, 2>, 3> tup3tup2f;

typedef tup<tup<int, 2>, 3> tup3tup2i;

typedef tup<float, 2> tup2f;

typedef tup<float, 3> tup3f;

typedef tup<float, 4> tup4f;

typedef tup<int, 2> tup2i;

typedef tup<int, 3> tup3i;

typedef tup<int, 4> tup4i;

typedef tup<uint8_t, 3> tup3ui8;
