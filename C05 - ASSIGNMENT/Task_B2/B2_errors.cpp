// B2: engineered error conditions. Every object prints from its destructor.
#include "../Task_B1/billing.hpp"
#include <new>
#include <string>

struct Tracker {                                        // proves RAII cleanup during unwinding
    std::string n; explicit Tracker(std::string s) : n(std::move(s)) { std::cout << "  +" << n << '\n'; }
    ~Tracker() { std::cout << "  ~" << n << '\n'; }
};

void run(const std::string& title, void (*test)())
{
    std::cout << "[" << title << "]\n";
    try { test(); std::cout << "  no error\n"; }
    catch (const NegativeChargeError& e) { std::cout << "  NegativeChargeError: " << e.what() << '\n'; }
    catch (const ClaimError& e)          { std::cout << "  ClaimError: " << e.what() << '\n'; }
    catch (const UnknownPackageError& e) { std::cout << "  UnknownPackageError: " << e.what() << '\n'; }
    catch (const PatientRecordError& e)  { std::cout << "  PatientRecordError: " << e.what() << '\n'; }
    catch (const BillingError& e)        { std::cout << "  BillingError: " << e.what() << '\n'; }
    catch (const std::bad_alloc& e)      { std::cout << "  std::bad_alloc: " << e.what() << '\n'; }
    catch (const std::invalid_argument& e){ std::cout << "  std::invalid_argument: " << e.what() << '\n'; }
    catch (const std::exception& e)      { std::cout << "  std::exception: " << e.what() << '\n'; }
}

int main()
{
    run("negative consultation charge", [] { Tracker t("tracker1"); OutPatientBill b("Ravi", -500, 100); });
    run("negative number of days",       [] { Tracker t("tracker2"); InPatientBill b("Meena", -1, 2500, 100, 100); });
    run("claim above cover", [] {
        Tracker t("tracker3");
        InPatientBill b("Meena", 3, 2500, 40000, 3500);                  // 51000
        Insurance ins(30000, 20);                                         // cover only 30000
        ins.amountCovered(b); });
    run("unknown package code", [] {
        Tracker t("tracker4");
        DayCareBill b("Arun", "ZZZ99", 25000);
        GovernmentScheme g({{"CAT01", 22000}});
        g.amountCovered(b); });
    run("missing patient record", [] {
        Tracker t("tracker5");
        PatientRegistry r; r.add("Ravi", 101);
        r.find("Unknown Person"); });
    run("stod accepts a numeric prefix (\"12abc\") -> must check pos", [] {
        Tracker t("tracker6");
        std::size_t pos = 0; std::string in = "12abc";
        double v = std::stod(in, &pos);
        std::cout << "  stod(\"12abc\") returned " << v << " with no exception, consumed " << pos << " of " << in.size() << " chars\n";
        if (pos != in.size()) throw std::invalid_argument("trailing characters in number"); });
    run("stod on pure text", [] { Tracker t("tracker7"); (void)std::stod("hello"); });
#ifndef SKIP_BADALLOC   // ASan aborts on >1 TB requests instead of throwing; see report
    run("memory allocation failure", [] {
        Tracker t("tracker8");
        volatile std::size_t huge = std::size_t(1) << 62;                    // ~4.6e18 bytes: impossible
        char* p = new char[huge];
        delete[] p; });

#endif

    std::cout << "[top-level catch(...) keeps the program alive]\n";
    try {
        Tracker outer("outer");
        Tracker inner("inner");
        throw 3.14;                                                       // unexpected type
    } catch (...) {
        std::cout << "  catch(...) reached; program continues\n";
    }
    std::cout << "program still running -> finished normally\n";
}
