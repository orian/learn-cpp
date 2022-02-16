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

int rnd() {
    static std::mt19937 mt_rand(std::time(0));
    return mt_rand();
}

void runBenchmarks(const size_t n, const int divisor_val) {
    std::vector<int> dividend(n), divisor(n, divisor_val), result(n);
    std::generate(dividend.begin(), dividend.end(), rnd);

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
    eval([n, &dividend, &divisor, &result] {
        for (size_t i = 0; i < n; ++i) {
            result[i] = dividend[i] % divisor[i];
        }
        return std::pair{"3[] built in %", result};
    });
    eval([n, &dividend, &divisor, &result] {
        auto a = dividend.data();
        auto b = divisor.data();
        auto c = result.data();

        for (size_t i = 0; i < n; ++i) {
            *(c++) = *(a++) % *(b++);
        }
        return std::pair{"3[] built in % pointers for data", result};
    });
    eval([n, &dividend, &result, divisor_val] {
        for (size_t i = 0; i < n; ++i) {
            result[i] = dividend[i] % divisor_val;
        }
        return std::pair{"2[] built in % const", result};
    });
    eval([n, &dividend, &result, divisor_val] {
        auto r = result.begin();
        auto begin = dividend.cbegin();
        for (size_t i = n; i; --i) {
            *(r++) = *(begin++) % divisor_val;
        }
        return std::pair{"2[] built in % const iter", result};
    });
    eval([n, &dividend, &result, divisor_val] {
        auto d = dividend.data();
        auto r = result.data();

        for (size_t i = n; i; --i) {
            *(r++) = *(d++) % divisor_val;
        }
        return std::pair{"2[] built in % const with raw pointer", result};
    });
    eval([n, &dividend, &result, divisor_val] {
        auto d = dividend.data();
        auto r = result.data();

        for (size_t i = n; i; --i) {
            *(r++) = *d >= divisor_val ? *(d++) % divisor_val : *(d++);
        }
        return std::pair{"2[] built in % const with raw pointer with IF", result};
    });
    eval([n, &dividend, &result, divisor_val] {
        auto d = dividend.data();
        auto r = result.data();

        uint64_t M = fastmod::computeM_u32(divisor_val);
        libdivide::divider<int> divider(divisor_val);
        for (size_t i = n; i; --i) {
            *(r++) = fastmod::fastmod_s32(*(d++), M, divisor_val);
        }
        return std::pair{"2[] fastmod", result};
    });

    result = dividend;
    eval([n, &result, divisor_val] {
        auto r = result.data();

        uint64_t M = fastmod::computeM_u32(divisor_val);
        libdivide::divider<int> divider(divisor_val);
        for (size_t i = n; i; --i) {
            *(r++) = fastmod::fastmod_s32(*r, M, divisor_val);
        }
        return std::pair{"1[] fastmod", result};
    });

    std::cout << std::reduce(result.cbegin(), result.cend()) << std::endl;
}

int main() {
    std::cout << "checksum: " << rnd() << std::endl;
    constexpr size_t k_n = 100000000;
    constexpr int k_divisor_val = 1398269;
    runBenchmarks(k_n, k_divisor_val);

    size_t n = 0;
    int divisor_val = 0; // mersenne prime num: 1398269
    std::cout << "provide divisor: ";
    std::cin >> n >> divisor_val;
    runBenchmarks(n, divisor_val);
}