#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>

namespace debug {

template <typename T>
void logl(const T& t) {
    std::cout << t << std::endl;
}

template <typename T>
void errl(const T& t) {
    std::cerr << t << std::endl;
}

template <typename T>
void log(const T& t) {
    std::cout << t;
}

template <typename T>
void err(const T& t) {
    std::cerr << t;
}

}

#endif // DEBUG_HPP
