#pragma once
#include <iostream>
#include <vector>

using std::vector;

template<typename T>
inline void print_vec(vector<T> in) {
    for (T thing : in)
        std::cout << thing << ", ";
    std::cout << "|||endvec\n";
};