// B1.1: one bill of each kind with each payer; totals verified against hand-computed values.
#include "billing.hpp"
#include <cmath>
#include <vector>

int main()
{
    std::vector<std::unique_ptr<Bill>> bills;
    bills.emplace_back(new OutPatientBill("Ravi", 500, 1200));            // 1700
    bills.emplace_back(new InPatientBill("Meena", 3, 2500, 40000, 3500)); // 51000
    bills.emplace_back(new DayCareBill("Arun", "CAT01", 25000));          // 25000
    bills.emplace_back(new EmergencyBill("Priya", 3000, 4, 800));         // 6200

    std::vector<std::unique_ptr<Payer>> payers;
    payers.emplace_back(new SelfPay);
    payers.emplace_back(new Insurance(60000, 20));
    payers.emplace_back(new GovernmentScheme({{"OP01", 1000}, {"IP01", 45000}, {"CAT01", 22000}, {"EMG01", 5000}}));

    const double expectTotal[4] = {1700, 51000, 25000, 6200};
    const double expectCover[3][4] = {                                    // hand-computed
        {0, 0, 0, 0},
        {1360, 40800, 20000, 4960},
        {1000, 45000, 22000, 5000}};

    int pass = 0, fail = 0; double day = 0;
    for (auto& b : bills) { b->print(); day += b->total(); }
    std::cout << "Day total = " << day << "  (hand: 83900)\n\n";
    std::cout << "payer       bill  total   covered  patientPays  expected  result\n";
    for (int p = 0; p < 3; ++p)
        for (int i = 0; i < 4; ++i) {
            double cov = payers[p]->amountCovered(*bills[i]);
            bool ok = std::fabs(bills[i]->total() - expectTotal[i]) < 1e-9 && std::fabs(cov - expectCover[p][i]) < 1e-9;
            (ok ? pass : fail)++;
            std::cout << payers[p]->name() << std::string(12 - payers[p]->name().size(), ' ')
                      << bills[i]->type() << "    " << bills[i]->total() << "   " << cov << "   "
                      << patientPays(*bills[i], *payers[p]) << "   " << expectCover[p][i]
                      << "   " << (ok ? "PASS" : "FAIL") << '\n';
        }
    std::cout << "\nPassed " << pass << ", failed " << fail << '\n';
    return fail ? 1 : 0;
}
