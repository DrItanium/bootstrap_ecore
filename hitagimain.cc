//
// Created by jwscoggins on 6/7/21.
//
#include <stdint.h>

template<typename T>
inline volatile T& memory(const uint32_t address) { return *reinterpret_cast<T*>(address); }

int main() {
    return 0;
}

