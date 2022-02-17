#pragma once

#include <chrono>
#include <ctime>
#include <iostream>
#include <random>


#ifndef LEARN_CPP_EVAL_H
#define LEARN_CPP_EVAL_H

int32_t seq() {
    static int32_t i;
    return ++i;
}

int64_t rnd() {
    static std::mt19937 mt_rand(std::time(0));
    return mt_rand();
}

auto eval(auto fun) {
    const auto t1 = std::chrono::high_resolution_clock::now();
    const auto[name, result] = fun();
    const auto t2 = std::chrono::high_resolution_clock::now();
    const auto sum = std::reduce(result.cbegin(), result.cend());
    const std::chrono::duration<double, std::milli> ms = t2 - t1;
    std::cout << std::fixed << name << " result "
              << sum << " took " << ms.count() << " ms\n";
};

#endif //LEARN_CPP_EVAL_H
