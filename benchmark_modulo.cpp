#include <iostream>
#include <random>
#include <ctime>
#include <chrono>
#include "libdivide.h"
#include "fastmod.h"

int f() {
    static int i;
    return ++i;
}

int main() {
    std::mt19937 mt_rand(std::time(0));

    std::cout << mt_rand() << std::endl;

    constexpr auto n = 100000000;
    constexpr auto divisor_val = 1398269; // mersenne prime num
    std::vector<int> dividend(n), divisor(n, divisor_val), result(n);
    std::generate(dividend.begin(), dividend.end(), f);
//    std::generate(divisor.begin(), divisor.end(), f);

    auto eval = [](auto fun) {
        const auto t1 = std::chrono::high_resolution_clock::now();
        const auto[name, result] = fun();
        const auto sum = std::reduce(result.cbegin(), result.cend());
        const auto t2 = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double, std::milli> ms = t2 - t1;
        std::cout << std::fixed << name << " result "
                  << sum << " took " << ms.count() << " ms\n";
    };

//    eval([&dividend, &divisor, &result] {
//        for (size_t i = 0; i < n; ++i) {
//            result[i] = dividend[i] % divisor[i];
//        }
//        return std::pair{"built in /", result};
//    });
    eval([&dividend, &divisor, &result] {
        for (size_t i = 0; i < n; ++i) {
            result[i] = dividend[i] % divisor[i];
        }
        return std::pair{"built in %", result};
    });
    eval([&dividend, &divisor, &result] {
        auto a = dividend.data();
        auto b = divisor.data();
        auto c = result.data();

        for (size_t i = 0; i < n; ++i) {
            *(c++) = *(a++) % *(b++);
        }
        return std::pair{"built in % pointers for data", result};
    });
    eval([&dividend, &result] {
        for (size_t i = 0; i < n; ++i) {
            result[i] = dividend[i] % divisor_val;
        }
        return std::pair{"built in % const", result};
    });
    eval([&dividend, &result] {
        auto r = result.begin();
        auto begin = dividend.cbegin();
        for (size_t i = n; i; --i) {
            *(r++) = *(begin++) % divisor_val;
        }
        return std::pair{"built in % const iter", result};
    });
    eval([&dividend, &result] {
        auto d = dividend.data();
        auto r = result.data();

        for (size_t i = n; i; --i) {
            *(r++) = *(d++) % divisor_val;
        }
        return std::pair{"built in % const with raw pointer", result};
    });
    eval([&dividend, &result] {
        auto d = dividend.data();
        auto r = result.data();

        for (size_t i = n; i; --i) {
            *(r++) = *d >= divisor_val ? *(d++) % divisor_val : *(d++);
        }
        return std::pair{"built in % const with raw pointer", result};
    });
    eval([&dividend, &result] {
        auto d = dividend.data();
        auto r = result.data();

        uint64_t M = fastmod::computeM_u32(divisor_val);
        libdivide::divider<int> divider(divisor_val);
        for (size_t i = n; i; --i) {
            *(r++) = fastmod::fastmod_s32(*(d++), M, divisor_val);
        }
        return std::pair{"fastmod", result};
    });

    std::cout << std::reduce(result.cbegin(), result.cend()) << std::endl;
}