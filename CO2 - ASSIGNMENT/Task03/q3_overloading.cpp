#include <iostream>
#include <string>
using namespace std;

struct Parcel {
    double weightKg;
    bool express;
    bool insured;
};

double charge(double weightKg, bool express = false, bool insured = false) {
    double total = weightKg * 40.0;
    if (express) total += 50.0;
    if (insured) total += 20.0;
    cout << "kg overload; ";
    return total;
}
double charge(int weightGrams) {
    cout << "grams overload; ";
    return (weightGrams / 1000.0) * 40.0;
}
double charge(const Parcel& p) {
    cout << "Parcel overload; ";
    return charge(p.weightKg, p.express, p.insured);
}

int main() {
    cout << "charge(2) = "; cout << charge(2) << '\n';
    cout << "charge(2.0) = "; cout << charge(2.0) << '\n';
    cout << "charge(2.0f) = "; cout << charge(2.0f) << '\n';
    cout << "charge(2000) = "; cout << charge(2000) << '\n';
    cout << "charge(2.0, true) = "; cout << charge(2.0, true) << '\n';
    Parcel p{2.0, true, true};
    cout << "charge(p) = "; cout << charge(p) << '\n';
}
