// B3.1: same 10 bills totalled by (a) virtual functions and (b) one struct + type field + switch.
#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
struct VBill { virtual ~VBill() = default; virtual double total() const = 0; };
struct VOP : VBill { double c, t; VOP(double a, double b) : c(a), t(b) {} double total() const override { return c + t; } };
struct VIP : VBill { int d; double r, p, m; VIP(int a, double b, double c_, double e) : d(a), r(b), p(c_), m(e) {} double total() const override { return d * r + p + m; } };
struct VDC : VBill { double pkg; explicit VDC(double a) : pkg(a) {} double total() const override { return pkg; } };
struct VEM : VBill { double base, h, rate; VEM(double a, double b, double c_) : base(a), h(b), rate(c_) {} double total() const override { return base + h * rate; } };
enum class Kind { OP, IP, DC, EM };
struct SBill { Kind k; int days; double a, b, c; };
double totalSwitch(const SBill& s) {
    switch (s.k) { case Kind::OP: return s.a + s.b; case Kind::IP: return s.days * s.a + s.b + s.c;
                   case Kind::DC: return s.a; default: return s.a + s.b * s.c; }
}
int main()
{
    std::vector<std::unique_ptr<VBill>> v; std::vector<SBill> s;
    v.emplace_back(new VOP(500, 1200));        s.push_back({Kind::OP, 0, 500, 1200, 0});        // 1700
    v.emplace_back(new VOP(300, 0));           s.push_back({Kind::OP, 0, 300, 0, 0});           // 300
    v.emplace_back(new VIP(3, 2500, 40000, 3500)); s.push_back({Kind::IP, 3, 2500, 40000, 3500}); // 51000
    v.emplace_back(new VIP(1, 2000, 0, 800));  s.push_back({Kind::IP, 1, 2000, 0, 800});        // 2800
    v.emplace_back(new VIP(5, 3000, 12000, 6000)); s.push_back({Kind::IP, 5, 3000, 12000, 6000}); // 33000
    v.emplace_back(new VDC(25000));            s.push_back({Kind::DC, 0, 25000, 0, 0});         // 25000
    v.emplace_back(new VDC(18000));            s.push_back({Kind::DC, 0, 18000, 0, 0});         // 18000
    v.emplace_back(new VEM(3000, 4, 800));     s.push_back({Kind::EM, 0, 3000, 4, 800});        // 6200
    v.emplace_back(new VEM(3000, 0.5, 800));   s.push_back({Kind::EM, 0, 3000, 0.5, 800});      // 3400
    v.emplace_back(new VEM(5000, 10, 900));    s.push_back({Kind::EM, 0, 5000, 10, 900});       // 14000
    const double hand[10] = {1700, 300, 51000, 2800, 33000, 25000, 18000, 6200, 3400, 14000};
    double sv = 0, ss = 0, sh = 0; bool allOk = true;
    for (int i = 0; i < 10; ++i) {
        double a = v[i]->total(), b = totalSwitch(s[i]);
        bool ok = std::fabs(a - b) < 1e-9 && std::fabs(a - hand[i]) < 1e-9; allOk &= ok;
        std::cout << "bill " << i + 1 << ": virtual=" << a << " switch=" << b << " hand=" << hand[i] << (ok ? "  OK\n" : "  MISMATCH\n");
        sv += a; ss += b; sh += hand[i];
    }
    std::cout << "Totals: virtual=" << sv << " switch=" << ss << " hand=" << sh << (allOk ? "  -> ALL AGREE\n" : "  -> DISAGREE\n");
}
