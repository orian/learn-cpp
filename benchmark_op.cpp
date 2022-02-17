#include <iostream>
#include <utility>
#include "eval.h"

void runBenchmark(const size_t n) {
    std::vector<int32_t> left_arg(n);
    std::vector<int64_t> right_arg(n), result(n, 0);
    std::generate(left_arg.begin(), left_arg.end(), seq);
    std::generate(right_arg.begin(), right_arg.end(), seq);

    eval([n, &left_arg, &right_arg, &result] {
        std::vector<int64_t> casted(left_arg.cbegin(), left_arg.cend());;
        auto r = result.data(), a = casted.data(), b = right_arg.data();
        for (size_t i = 0; i < n; ++i) {
            *(r++) = *(a++) + *(b++);
        }
        return std::pair{"ADD with cast column", result};
    });
    eval([&left_arg, &right_arg, &result] {
        auto op = [](const std::vector<int32_t> &left, const std::vector<int64_t> &right,
                     std::vector<int64_t> &result) {
            auto r = result.data();
            auto a = left.data();
            auto b = right.data();
            const auto n = result.size();
            for (size_t i = 0; i < n; ++i) {
                *(r++) = *(a++) + *(b++);
            }
        };
        op(left_arg, right_arg, result);
        return std::pair{"ADD with lambda", result};
    });

    eval([n, &left_arg, &right_arg, &result] {
        std::vector<int64_t> casted(left_arg.cbegin(), left_arg.cend());;
        auto r = result.data(), a = casted.data(), b = right_arg.data();
        for (size_t i = 0; i < n; ++i) {
            *(r++) = *(a++) * (*(b++));
        }
        return std::pair{"MUL with cast column", result};
    });
    eval([&left_arg, &right_arg, &result] {
        auto op = [](const std::vector<int32_t> &left, const std::vector<int64_t> &right,
                     std::vector<int64_t> &result) {
            auto r = result.data();
            auto a = left.data();
            auto b = right.data();
            const auto n = result.size();
            for (size_t i = 0; i < n; ++i) {
                *(r++) = *(a++) * (*(b++));
            }
        };
        op(left_arg, right_arg, result);
        return std::pair{"MUL with lambda", result};
    });
}

int main() {
    std::cout << "checksum: " << rnd() << std::endl;

    size_t n = 0;
    std::cout << "provide divisor: ";
    std::cin >> n;
    runBenchmark(n);
}
