#include <iostream>
#include <array>
#include <chrono>
using namespace std;
using Clock = chrono::steady_clock;
constexpr int K = 10;
constexpr int N = 2000000;

class ShiftHistory {
    array<int, K> data{};
    int count = 0;
public:
    void add(int value) {
        if (count < K) data[count++] = value;
        else {
            for (int i = 1; i < K; ++i) data[i - 1] = data[i];
            data[K - 1] = value;
        }
    }
    int last() const { return data[K - 1]; }
};
class CircularHistory {
    array<int, K> data{};
    int next = 0;
    int count = 0;
public:
    void add(int value) {
        data[next] = value;
        next = (next + 1) % K;
        if (count < K) ++count;
    }
    int last() const { return data[(next + K - 1) % K]; }
};
template<class T>
long long benchmark() {
    T history;
    auto start = Clock::now();
    for (int i = 0; i < N; ++i) history.add(i);
    auto stop = Clock::now();
    volatile int keep = history.last();
    (void)keep;
    return chrono::duration_cast<chrono::microseconds>(stop - start).count();
}
int main() {
    ShiftHistory s;
    CircularHistory c;
    for (int i = 1; i <= 12; ++i) { s.add(i); c.add(i); }
    cout << "After adding 1..12, newest value: shift=" << s.last()
         << ", circular=" << c.last() << '\n';
    cout << "Benchmark inserts=" << N << ", capacity=" << K << '\n';
    cout << "Shift microseconds: " << benchmark<ShiftHistory>() << '\n';
    cout << "Circular microseconds: " << benchmark<CircularHistory>() << '\n';
    cout << "Timing varies by machine/compiler/load; run repeatedly and compare medians.\n";
}
