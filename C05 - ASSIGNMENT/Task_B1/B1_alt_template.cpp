// Alternative concept: compile-time polymorphism -- one container per bill type, templates.
#include <iostream>
#include <vector>
struct OP { double c, t;           double total() const { return c + t; } };
struct IP { int d; double r, s;    double total() const { return d * r + s; } };
template <class B> double sumAll(const std::vector<B>& v) {   // no virtual call, can be inlined
    double s = 0; for (const auto& b : v) s += b.total(); return s;
}
int main()
{
    std::vector<OP> ops = { {500, 1200}, {300, 0} };
    std::vector<IP> ips = { {3, 2500, 40000} };
    std::cout << "OP total " << sumAll(ops) << ", IP total " << sumAll(ips)
              << ", day total " << sumAll(ops) + sumAll(ips) << '\n';
    // std::vector<???> all;  // one list of OP and IP together is impossible without a common base/variant
    std::cout << "sizeof(OP)=" << sizeof(OP) << " sizeof(IP)=" << sizeof(IP) << " (no vptr)\n";
}
