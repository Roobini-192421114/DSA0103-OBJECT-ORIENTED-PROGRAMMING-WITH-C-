// Alternative concept: payers as std::function policies instead of an abstract Payer class.
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
using Policy = std::function<double(double /*billTotal*/)>;

Policy selfPay()                         { return [](double)   { return 0.0; }; }
Policy insurance(double limit, double copayPct) {            // limit/copay live in the closure
    return [=](double t) { return std::min(t, limit) * (1 - copayPct / 100); };
}
Policy government(double packageRate)    { return [=](double t) { return std::min(t, packageRate); }; }

int main()
{
    Policy payers[] = { selfPay(), insurance(60000, 20), government(45000) };
    std::string names[] = { "SelfPay", "Insurance(60000,20%)", "Government(45000)" };
    double total = 51000;
    for (int i = 0; i < 3; ++i)
        std::cout << names[i] << " covers " << payers[i](total) << '\n';
    std::cout << "sizeof(Policy)=" << sizeof(Policy) << " bytes per payer (captures are in the closure)\n";
    Policy corporate = [](double t) { return std::min(t, 10000.0); };
    std::cout << "Corporate covers " << corporate(total) << '\n';
}
