// Alternative concept: manual delete in every catch block (error-prone) vs RAII.
#include <iostream>
#include <memory>
#include <stdexcept>
void mayThrow(int k) { if (k == 1) throw std::runtime_error("E1"); if (k == 2) throw std::logic_error("E2"); }

void manual(int k) {                       // new error path => one more place to remember delete
    int* a = new int[10];
    double* b = nullptr;
    try {
        b = new double[10];
        mayThrow(k);
        delete[] a; delete[] b;
    } catch (const std::runtime_error&) { delete[] a; delete[] b; throw; }
    catch (const std::logic_error&)     { delete[] a; /* BUG: forgot delete[] b -> LEAK */ throw; }
}
void raii(int k) {                          // zero cleanup code, every path is safe
    auto a = std::make_unique<int[]>(10);
    auto b = std::make_unique<double[]>(10);
    mayThrow(k);
}
int main()
{
    for (int k = 0; k < 3; ++k) {
        try { manual(k); } catch (const std::exception& e) { std::cout << "manual caught " << e.what() << '\n'; }
        try { raii(k); }   catch (const std::exception& e) { std::cout << "raii   caught " << e.what() << '\n'; }
    }
}
