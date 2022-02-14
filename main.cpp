#include <iostream>
#include <vector>
#include <execution>

class Tuple {
public:
    const int subject;
    const int predicate;
    const int object;

    Tuple(int subject, int predicate, int object) : subject(subject), predicate(predicate), object(object) {}
};

void PrintElapsed(std::chrono::steady_clock::time_point start,
                  std::chrono::steady_clock::time_point end) {
    using namespace std;
//    cout << "Elapsed time in nanoseconds: "
//         << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
//         << " ns" << endl;

    cout << "Elapsed time in microseconds: "
         << chrono::duration_cast<chrono::microseconds>(end - start).count()
         << " µs" << endl;

//    cout << "Elapsed time in milliseconds: "
//         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
//         << " ms" << endl;
//
//    cout << "Elapsed time in seconds: "
//         << chrono::duration_cast<chrono::seconds>(end - start).count()
//         << " sec" << endl;
}

int main() {
    srand(314);

    constexpr auto N = 80000000;
    std::vector<Tuple *> data;
    int sub = 1;
    for (int i = 0; i < N; i++) {
        data.push_back(new Tuple(i, i, i));
    }
    std::cout << "data size: " << data.size() << std::endl
              << "number to find: ";
    int find_v;
    int object;
    std::cin >> find_v;
    std::chrono::steady_clock::time_point start, end;

    for (int a = 0; a < 5; ++a) {
        start = std::chrono::steady_clock::now();
        std::for_each(data.begin(), data.end(), [find_v, &object](const auto t) {
            if (t->subject == find_v) {
                object = t->object;
            }
        });
        end = std::chrono::steady_clock::now();
        std::cout << object;
        PrintElapsed(start, end);

        start = std::chrono::steady_clock::now();
        for (const auto &d: data) {
            if (d->subject == find_v) {
//            std::cout << "found!" << std::endl;
                object = d->object;
            }
        }
        end = std::chrono::steady_clock::now();
        std::cout << object;
        PrintElapsed(start, end);

        start = std::chrono::steady_clock::now();
        for (auto d = data.cbegin(); d != data.cend(); ++d) {
            if ((*d)->subject == find_v) {
                object = (*d)->object;
            }
        }
        end = std::chrono::steady_clock::now();
        std::cout << object;
        PrintElapsed(start, end);

        // standard loop
        start = std::chrono::steady_clock::now();
        for (size_t i = 0, s = data.size(); i < s; ++i) {
            const auto d = data[i];
            if (d->subject == find_v) {
                object = d->object;
            }
        }
        end = std::chrono::steady_clock::now();
        std::cout << object;
        PrintElapsed(start, end);

        // standard loop
        start = std::chrono::steady_clock::now();
        for (int i = 0; i < data.size(); ++i) {
            const auto d = data[i];
            if (d->subject == find_v) {
//            std::cout << "found!" << std::endl;
                object = d->object;
            }
        }
        end = std::chrono::steady_clock::now();
        std::cout << object;
        PrintElapsed(start, end);
        std::cout << "iter " << a << std::endl;
    }
}
