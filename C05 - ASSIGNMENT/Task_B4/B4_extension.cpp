// B4.1: add ONE new bill type (TeleConsultBill) and ONE new payer (CorporatePayer)
// WITHOUT editing billing.hpp or any existing class. Old tests are re-run unchanged.
#include "../Task_B1/billing.hpp"
#include <cmath>
#include <vector>

// ---- new bill type: only derives from Bill and overrides the pure virtuals
class TeleConsultBill : public Bill {
    double fee_, minutes_, perMinute_;
public:
    TeleConsultBill(std::string p, double fee, double minutes, double perMinute)
        : Bill(std::move(p), "TELE01"), fee_(nonNeg(fee, "fee")), minutes_(nonNeg(minutes, "minutes")), perMinute_(nonNeg(perMinute, "per-minute rate")) {}
    double total() const override { return fee_ + minutes_ * perMinute_; }
    std::string type() const override { return "TC"; }
};

// ---- new payer: corporate tie-up, covers 100% up to a per-bill cap, employee must be registered
class CorporatePayer : public Payer {
    double perBillCap_; std::string company_;
public:
    CorporatePayer(std::string company, double cap) : perBillCap_(nonNeg(cap, "cap")), company_(std::move(company)) {}
    double amountCovered(const Bill& b) const override { return std::min(b.total(), perBillCap_); }
    std::string name() const override { return "Corporate(" + company_ + ")"; }
};

int main()
{
    int pass = 0, fail = 0;
    auto check = [&](const char* what, double got, double want) {
        bool ok = std::fabs(got - want) < 1e-9; (ok ? pass : fail)++;
        std::cout << (ok ? "PASS " : "FAIL ") << what << ": got " << got << ", expected " << want << '\n';
    };

    // ---- earlier tests (identical to B1_billing_tests, 12 combinations)
    std::vector<std::unique_ptr<Bill>> bills;
    bills.emplace_back(new OutPatientBill("Ravi", 500, 1200));
    bills.emplace_back(new InPatientBill("Meena", 3, 2500, 40000, 3500));
    bills.emplace_back(new DayCareBill("Arun", "CAT01", 25000));
    bills.emplace_back(new EmergencyBill("Priya", 3000, 4, 800));
    SelfPay sp; Insurance ins(60000, 20);
    GovernmentScheme gov({{"OP01", 1000}, {"IP01", 45000}, {"CAT01", 22000}, {"EMG01", 5000}, {"TELE01", 200}});
    const double eIns[4] = {1360, 40800, 20000, 4960}, eGov[4] = {1000, 45000, 22000, 5000};
    for (int i = 0; i < 4; ++i) {
        check("old: SelfPay covered", sp.amountCovered(*bills[i]), 0);
        check("old: Insurance covered", ins.amountCovered(*bills[i]), eIns[i]);
        check("old: Government covered", gov.amountCovered(*bills[i]), eGov[i]);
    }

    // ---- new bill with ALL old payers, and ALL old bills with the new payer
    TeleConsultBill tc("Kavya", 300, 20, 10);                 // 300 + 20*10 = 500
    bills.emplace_back(new TeleConsultBill("Kavya", 300, 20, 10));
    check("new bill total", tc.total(), 500);
    check("new bill + SelfPay", sp.amountCovered(tc), 0);
    check("new bill + Insurance(20%)", ins.amountCovered(tc), 400);
    check("new bill + Government(TELE01=200)", gov.amountCovered(tc), 200);
    CorporatePayer corp("Acme Ltd", 10000);
    const double eCorp[5] = {1700, 10000, 10000, 6200, 500};  // min(total, 10000)
    for (int i = 0; i < 5; ++i) check("new payer on each bill", corp.amountCovered(*bills[i]), eCorp[i]);

    // ---- thrown and caught exceptions in the same run (for the sanitizer report)
    int caught = 0;
    try { TeleConsultBill bad("X", -1, 5, 5); }           catch (const NegativeChargeError& e) { ++caught; std::cout << "caught NegativeChargeError: " << e.what() << '\n'; }
    try { Insurance small(100, 0); small.amountCovered(tc); } catch (const ClaimError& e)          { ++caught; std::cout << "caught ClaimError: " << e.what() << '\n'; }
    try { GovernmentScheme g({}); g.amountCovered(tc); }   catch (const UnknownPackageError& e)    { ++caught; std::cout << "caught UnknownPackageError: " << e.what() << '\n'; }
    try { PatientRegistry r; r.find("Ghost"); }            catch (const BillingError& e)           { ++caught; std::cout << "caught BillingError: " << e.what() << '\n'; }
    check("exceptions caught", caught, 4);

    std::cout << "\nPassed " << pass << ", failed " << fail << '\n';
    return fail ? 1 : 0;
}
