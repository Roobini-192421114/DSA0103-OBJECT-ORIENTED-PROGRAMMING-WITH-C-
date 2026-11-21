// Alternative concept: total() NOT virtual; the caller discovers the type with dynamic_cast.
#include <chrono>
#include <iostream>
#include <memory>
#include <vector>
struct Bill { virtual ~Bill() = default; };            // still polymorphic (needed by dynamic_cast)
struct OP : Bill { double c = 500, t = 1200; };
struct IP : Bill { int d = 3; double r = 2500, s = 40000; };
struct DC : Bill { double pkg = 25000; };
struct EM : Bill { double base = 3000, h = 4, rate = 800; };

double totalCast(const Bill* b) {                       // up to k = 4 casts
    if (auto p = dynamic_cast<const OP*>(b)) return p->c + p->t;
    if (auto p = dynamic_cast<const IP*>(b)) return p->d * p->r + p->s;
    if (auto p = dynamic_cast<const DC*>(b)) return p->pkg;
    if (auto p = dynamic_cast<const EM*>(b)) return p->base + p->h * p->rate;
    return 0;
}
struct VBill { virtual ~VBill() = default; virtual double total() const = 0; };
struct VOP : VBill { double total() const override { return 1700; } };
struct VIP : VBill { double total() const override { return 47500; } };
struct VDC : VBill { double total() const override { return 25000; } };
struct VEM : VBill { double total() const override { return 6200; } };

int main()
{
    const int N = 2000000;
    std::vector<std::unique_ptr<Bill>> a; std::vector<std::unique_ptr<VBill>> v;
    for (int i = 0; i < N; ++i) {
        switch (i % 4) {
        case 0: a.emplace_back(new OP); v.emplace_back(new VOP); break;
        case 1: a.emplace_back(new IP); v.emplace_back(new VIP); break;
        case 2: a.emplace_back(new DC); v.emplace_back(new VDC); break;
        default: a.emplace_back(new EM); v.emplace_back(new VEM); }
    }
    using clk = std::chrono::steady_clock;
    double s1 = 0, s2 = 0;
    auto t0 = clk::now(); for (auto& p : a) s1 += totalCast(p.get());
    auto t1 = clk::now(); for (auto& p : v) s2 += p->total();
    auto t2 = clk::now();
    auto ms = [](auto d) { return std::chrono::duration<double, std::milli>(d).count(); };
    std::cout << "dynamic_cast chain: sum=" << s1 << "  " << ms(t1 - t0) << " ms\n";
    std::cout << "virtual call      : sum=" << s2 << "  " << ms(t2 - t1) << " ms\n";
    std::cout << "sizeof(VBill)=" << sizeof(VBill) << " (one vptr)\n";
}
