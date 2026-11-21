// Alternative concept: errors reported by enum Status instead of exceptions.
#include <iostream>
#include <string>
enum class Status { Ok, NegativeCharge, ClaimAboveCover, UnknownPackage };
const char* msg(Status s) {
    switch (s) {
    case Status::Ok: return "ok";
    case Status::NegativeCharge: return "negative charge";
    case Status::ClaimAboveCover: return "claim above cover";
    default: return "unknown package"; }
}
Status approve(double billed, double cover, double& out) {   // result via out-parameter
    if (billed < 0 || cover < 0) return Status::NegativeCharge;
    if (billed > cover) return Status::ClaimAboveCover;
    out = billed; return Status::Ok;
}
Status settle(double billed, double cover, double& out) {    // every layer must forward the code
    Status s = approve(billed, cover, out);
    if (s != Status::Ok) { std::cout << "audit: " << msg(s) << '\n'; return s; }
    return s;
}
int main()
{
    double cases[][2] = {{40000, 50000}, {60000, 50000}, {-1, 50000}};
    for (auto& c : cases) {
        double paid = 0;
        Status s = settle(c[0], c[1], paid);                  // caller MUST remember to check
        if (s == Status::Ok) std::cout << "approved " << paid << '\n';
        else std::cout << "rejected: " << msg(s) << '\n';
    }
}
