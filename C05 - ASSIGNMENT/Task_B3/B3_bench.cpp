// B3: virtual functions vs switch vs std::variant; exceptions vs return codes vs std::optional.
// Build: g++ -std=c++17 -O2 -Wall -Wextra B3_bench.cpp -o b3
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <random>
#include <stdexcept>
#include <variant>
#include <vector>
#include <algorithm>
#include <iomanip>

using clk = std::chrono::steady_clock;
static double ms(clk::time_point a, clk::time_point b) { return std::chrono::duration<double, std::milli>(b - a).count(); }

// ------------------------------------------------ (a) virtual hierarchy
struct VBill { virtual ~VBill() = default; virtual double total() const = 0; };
struct VOP : VBill { double c, t;               VOP(double a, double b) : c(a), t(b) {}           double total() const override { return c + t; } };
struct VIP : VBill { int d; double r, p, m;     VIP(int a, double b, double c_, double e) : d(a), r(b), p(c_), m(e) {} double total() const override { return d * r + p + m; } };
struct VDC : VBill { double pkg;                explicit VDC(double a) : pkg(a) {}                 double total() const override { return pkg; } };
struct VEM : VBill { double base, h, rate;      VEM(double a, double b, double c_) : base(a), h(b), rate(c_) {} double total() const override { return base + h * rate; } };

// ------------------------------------------------ (b) one struct + type field + switch
enum class Kind : std::uint8_t { OP, IP, DC, EM };
struct SBill { Kind k; int days; double a, b, c; };       // a,b,c reused per kind -> fixed size
inline double totalSwitch(const SBill& s) {
    switch (s.k) {
    case Kind::OP: return s.a + s.b;
    case Kind::IP: return s.days * s.a + s.b + s.c;
    case Kind::DC: return s.a;
    default:       return s.a + s.b * s.c;
    }
}

// ------------------------------------------------ (c) std::variant
struct POP { double c, t;           double total() const { return c + t; } };
struct PIP { int d; double r, p, m; double total() const { return d * r + p + m; } };
struct PDC { double pkg;            double total() const { return pkg; } };
struct PEM { double base, h, rate;  double total() const { return base + h * rate; } };
using VarBill = std::variant<POP, PIP, PDC, PEM>;

struct Spec { int kind; double x, y, z; int d; };         // identical random data for all three

static std::vector<Spec> makeSpecs(std::size_t n) {
    std::mt19937 g(12345); std::uniform_int_distribution<int> k(0, 3); std::uniform_real_distribution<double> u(100, 5000);
    std::vector<Spec> v(n);
    for (auto& s : v) s = {k(g), u(g), u(g), u(g), int(u(g)) % 10 + 1};
    return v;
}

template <class F> static double bestOf(int reps, F f, double& sink) {
    double best = 1e300;
    for (int i = 0; i < reps; ++i) { auto t0 = clk::now(); sink += f(); auto t1 = clk::now(); best = std::min(best, ms(t0, t1)); }
    return best;
}

static void part1(std::size_t n, int reps) {
    auto specs = makeSpecs(n);
    std::vector<std::unique_ptr<VBill>> vb; vb.reserve(n);
    std::vector<SBill> sb; sb.reserve(n);
    std::vector<VarBill> vr; vr.reserve(n);
    for (auto& s : specs) {
        switch (s.kind) {
        case 0: vb.emplace_back(new VOP(s.x, s.y));                    sb.push_back({Kind::OP, 0, s.x, s.y, 0});          vr.emplace_back(POP{s.x, s.y}); break;
        case 1: vb.emplace_back(new VIP(s.d, s.x, s.y, s.z));          sb.push_back({Kind::IP, s.d, s.x, s.y, s.z});      vr.emplace_back(PIP{s.d, s.x, s.y, s.z}); break;
        case 2: vb.emplace_back(new VDC(s.x));                         sb.push_back({Kind::DC, 0, s.x, 0, 0});            vr.emplace_back(PDC{s.x}); break;
        default: vb.emplace_back(new VEM(s.x, s.y, s.z));              sb.push_back({Kind::EM, 0, s.x, s.y, s.z});        vr.emplace_back(PEM{s.x, s.y, s.z}); }
    }
    double sink = 0, sv = 0, ss = 0, sr = 0;
    double tv = bestOf(reps, [&] { double s = 0; for (auto& p : vb) s += p->total(); sv = s; return s; }, sink);
    double ts = bestOf(reps, [&] { double s = 0; for (auto& b : sb) s += totalSwitch(b); ss = s; return s; }, sink);
    double tr = bestOf(reps, [&] { double s = 0; for (auto& v : vr) s += std::visit([](const auto& b) { return b.total(); }, v); sr = s; return s; }, sink);
    std::cout << std::fixed << std::setprecision(3);
    std::cout << std::setw(10) << n << " | virtual " << std::setw(10) << tv << " ms | switch " << std::setw(10) << ts
              << " ms | variant " << std::setw(10) << tr << " ms | sums equal: "
              << ((std::abs(sv - ss) < 1e-3 * std::abs(sv) && std::abs(sv - sr) < 1e-3 * std::abs(sv)) ? "yes" : "NO") << '\n';
}

// ------------------------------------------------ validation: exceptions vs return codes vs optional
static double validateThrow(double x) { if (x < 0) throw std::invalid_argument("negative"); return x; }
enum class St { Ok, Neg };
static St validateCode(double x, double& out) { if (x < 0) return St::Neg; out = x; return St::Ok; }
static std::optional<double> validateOpt(double x) { if (x < 0) return std::nullopt; return x; }

static void part2(std::size_t n, int pctInvalid) {
    std::mt19937 g(777); std::uniform_int_distribution<int> pct(0, 9999);
    std::vector<double> in(n);
    for (auto& x : in) x = (pct(g) < pctInvalid * 100) ? -1.0 : 100.0;
    double sink = 0; std::size_t bad;
    auto t0 = clk::now(); bad = 0;
    for (double x : in) { double o; if (validateCode(x, o) == St::Ok) sink += o; else ++bad; }
    auto t1 = clk::now(); std::size_t b1 = bad; bad = 0;
    for (double x : in) { auto r = validateOpt(x); if (r) sink += *r; else ++bad; }
    auto t2 = clk::now(); std::size_t b2 = bad; bad = 0;
    for (double x : in) { try { sink += validateThrow(x); } catch (const std::invalid_argument&) { ++bad; } }
    auto t3 = clk::now();
    std::cout << std::fixed << std::setprecision(3) << std::setw(3) << pctInvalid << "% invalid | return code " << std::setw(9) << ms(t0, t1)
              << " ms | optional " << std::setw(9) << ms(t1, t2) << " ms | exception " << std::setw(10) << ms(t2, t3)
              << " ms | rejected " << b1 << "/" << b2 << "/" << bad << (sink < 0 ? "?" : "") << '\n';
}

int main()
{
    std::cout << "sizeof: VBill-derived (largest VIP)=" << sizeof(VIP) << " (+16 malloc rounding, + 8-byte pointer in vector), SBill=" << sizeof(SBill)
              << ", VarBill=" << sizeof(VarBill) << ", POP=" << sizeof(POP) << ", PIP=" << sizeof(PIP) << ", PEM=" << sizeof(PEM) << "\n\n";
    std::cout << "B3.2  total() over n bills, best of 5 runs (-O2)\n";
    part1(10, 5); part1(1000, 5); part1(1000000, 5); part1(10000000, 3);
    std::cout << "\nB3.3  validate 1,000,000 bills\n";
    for (int p : {0, 1, 50}) part2(1000000, p);
}
