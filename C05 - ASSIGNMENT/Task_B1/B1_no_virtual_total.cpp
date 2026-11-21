// B1.3: remove 'virtual' from total() (and give it a body, since "= 0" requires virtual).
#include <iostream>
#include <string>
struct Bill {
    std::string patient; explicit Bill(std::string p) : patient(std::move(p)) {}
    virtual ~Bill() = default;
    double total() const { return 0; }                         // NOT virtual -> early binding
    virtual std::string type() const { return "Bill"; }
    void print() const { std::cout << type() << ' ' << patient << ": " << total() << '\n'; }
};
struct OutPatient : Bill {
    double c, t; OutPatient(std::string p, double c_, double t_) : Bill(std::move(p)), c(c_), t(t_) {}
    double total() const { return c + t; }                     // hides, does not override
    std::string type() const override { return "OP"; }
};
struct InPatient : Bill {
    int d; double r, s; InPatient(std::string p, int d_, double r_, double s_) : Bill(std::move(p)), d(d_), r(r_), s(s_) {}
    double total() const { return d * r + s; }
    std::string type() const override { return "IP"; }
};
int main()
{
    Bill* bills[] = { new OutPatient("Ravi", 500, 1200), new InPatient("Meena", 3, 2500, 40000) };
    double sum = 0;
    for (Bill* b : bills) { b->print(); sum += b->total(); }
    std::cout << "Total " << sum << "   (expected 49200)\n";
    for (Bill* b : bills) delete b;
}
