// Alternative concept: bill types/payers loaded as RULES from configuration text, interpreted at run time.
#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using clk = std::chrono::steady_clock;

// ---- the "plug-in file": one line per bill type: name = fixed + a*x + b*y
static const char* CONFIG = R"(OP      = 0    1  1
IP      = 3500 2500 40000
DC      = 25000 0 0
EM      = 3000 800 4
TELE    = 300  10 20
)";
struct Rule { double fixed, a, b; };
double evalRule(const Rule& r, double x, double y) { return r.fixed + r.a * x + r.b * y; }   // interpreted form

// ---- compiled form for comparison
struct VBill { virtual ~VBill() = default; virtual double total() const = 0; };
struct VAny : VBill { double f, a, b, x, y; VAny(double f_, double a_, double b_, double x_, double y_) : f(f_), a(a_), b(b_), x(x_), y(y_) {}
                     double total() const override { return f + a * x + b * y; } };

int main()
{
    std::unordered_map<std::string, Rule> rules; std::istringstream in(CONFIG); std::string name, eq; Rule r;
    while (in >> name >> eq >> r.fixed >> r.a >> r.b) rules[name] = r;
    std::cout << "loaded " << rules.size() << " rules (adding TELE needed NO recompile)\n";

    const char* names[] = {"OP", "IP", "DC", "EM", "TELE"};
    const int N = 5000000;
    struct Item { const char* kind; double x, y; };
    std::vector<Item> items; std::vector<std::unique_ptr<VBill>> objs;
    for (int i = 0; i < N; ++i) { const char* k = names[i % 5]; items.push_back({k, 1.0 + i % 7, 2.0 + i % 3});
        const Rule& q = rules[k]; objs.emplace_back(new VAny(q.fixed, q.a, q.b, items.back().x, items.back().y)); }

    double s1 = 0, s2 = 0; auto t0 = clk::now();
    for (auto& it : items) s1 += evalRule(rules.find(it.kind)->second, it.x, it.y);      // hash lookup by name per bill
    auto t1 = clk::now();
    for (auto& o : objs) s2 += o->total();
    auto t2 = clk::now();
    auto ms = [](auto d) { return std::chrono::duration<double, std::milli>(d).count(); };
    std::cout << "rule interpretation (string lookup + eval): " << ms(t1 - t0) << " ms  sum=" << s1 << '\n';
    std::cout << "compiled virtual call                    : " << ms(t2 - t1) << " ms  sum=" << s2 << '\n';
    std::cout << "sizeof(Rule)=" << sizeof(Rule) << " bytes per rule type (N rule types, not N classes)\n";
}
