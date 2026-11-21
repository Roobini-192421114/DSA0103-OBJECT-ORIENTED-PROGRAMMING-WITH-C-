// billing.hpp -- Hospital billing: Bill hierarchy, Payer hierarchy, exception hierarchy.
#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

// ---------------------------------------------------------------- exceptions (A4)
class BillingError : public std::runtime_error {            // root of every billing error
public: using std::runtime_error::runtime_error;
};
class NegativeChargeError : public BillingError { public: using BillingError::BillingError; };
class ClaimError          : public BillingError { public: using BillingError::BillingError; };
class UnknownPackageError : public BillingError { public: using BillingError::BillingError; };
class PatientRecordError  : public BillingError { public: using BillingError::BillingError; };
// memory failure is reported by the standard std::bad_alloc (not wrapped).

inline double nonNeg(double v, const char* what) {           // shared validation helper
    if (v < 0) throw NegativeChargeError(std::string("negative ") + what);
    return v;
}

// ---------------------------------------------------------------- Bill hierarchy (A1)
class Bill {
protected:
    std::string patient_;
    std::string package_;                                     // package code ("" = none)
public:
    Bill(std::string patient, std::string package = "")
        : patient_(std::move(patient)), package_(std::move(package)) {}
    virtual ~Bill() = default;                                // virtual destructor (A2.3)
    virtual double      total() const = 0;                    // pure virtual
    virtual std::string type()  const = 0;                    // pure virtual
    virtual void        print(std::ostream& os = std::cout) const {   // virtual with default
        os << type() << ' ' << patient_ << ": " << total() << '\n';
    }
    const std::string& patient() const noexcept { return patient_; }
    const std::string& package() const noexcept { return package_; }
};

class OutPatientBill : public Bill {
    double consultation_, tests_;
public:
    OutPatientBill(std::string p, double consultation, double tests)
        : Bill(std::move(p), "OP01"),
          consultation_(nonNeg(consultation, "consultation")), tests_(nonNeg(tests, "tests")) {}
    double total() const override { return consultation_ + tests_; }
    std::string type() const override { return "OP"; }
};

class InPatientBill : public Bill {
    int days_; double perDay_, procedures_, medicines_;
public:
    InPatientBill(std::string p, int days, double perDay, double procedures, double medicines)
        : Bill(std::move(p), "IP01"), days_(days), perDay_(nonNeg(perDay, "room rate")),
          procedures_(nonNeg(procedures, "procedures")), medicines_(nonNeg(medicines, "medicines")) {
        if (days < 0) throw NegativeChargeError("negative days");
    }
    double total() const override { return days_ * perDay_ + procedures_ + medicines_; }
    std::string type() const override { return "IP"; }
};

class DayCareBill : public Bill {
    double package_price_;
public:
    DayCareBill(std::string p, std::string code, double price)
        : Bill(std::move(p), std::move(code)), package_price_(nonNeg(price, "package price")) {}
    double total() const override { return package_price_; }
    std::string type() const override { return "DC"; }
};

class EmergencyBill : public Bill {
    double base_, hours_, hourly_;
public:
    EmergencyBill(std::string p, double base, double hours, double hourly)
        : Bill(std::move(p), "EMG01"), base_(nonNeg(base, "base charge")),
          hours_(nonNeg(hours, "hours")), hourly_(nonNeg(hourly, "hourly charge")) {}
    double total() const override { return base_ + hours_ * hourly_; }
    std::string type() const override { return "EM"; }
};

// ---------------------------------------------------------------- Payer hierarchy (A3)
class Payer {
public:
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill& b) const = 0;    // pure virtual
    virtual std::string name() const = 0;
};

class SelfPay : public Payer {
public:
    double amountCovered(const Bill&) const override { return 0.0; }   // patient pays all
    std::string name() const override { return "SelfPay"; }
};

class Insurance : public Payer {
    double limit_, copayPct_;
public:
    Insurance(double coverLimit, double copayPercent)
        : limit_(nonNeg(coverLimit, "cover limit")), copayPct_(copayPercent) {
        if (copayPercent < 0 || copayPercent > 100) throw BillingError("co-payment must be 0..100");
    }
    double amountCovered(const Bill& b) const override {
        double t = b.total();
        if (t > limit_) throw ClaimError("claim " + std::to_string((long long)t) + " exceeds cover " +
                                         std::to_string((long long)limit_));
        return t * (1.0 - copayPct_ / 100.0);
    }
    std::string name() const override { return "Insurance"; }
};

class GovernmentScheme : public Payer {
    std::map<std::string, double> rates_;                      // package code -> fixed rate
public:
    explicit GovernmentScheme(std::map<std::string, double> rates) : rates_(std::move(rates)) {}
    double amountCovered(const Bill& b) const override {
        auto it = rates_.find(b.package());
        if (it == rates_.end()) throw UnknownPackageError("unknown package '" + b.package() + "'");
        return std::min(b.total(), it->second);                // fixed package rate caps the cover
    }
    std::string name() const override { return "Government"; }
};

// ---------------------------------------------------------------- patient registry (missing record)
class PatientRegistry {
    std::map<std::string, int> ids_;
public:
    void add(const std::string& n, int id) { ids_[n] = id; }
    int find(const std::string& n) const {
        auto it = ids_.find(n);
        if (it == ids_.end()) throw PatientRecordError("no record for patient '" + n + "'");
        return it->second;
    }
};

inline double patientPays(const Bill& b, const Payer& p) { return b.total() - p.amountCovered(b); }
