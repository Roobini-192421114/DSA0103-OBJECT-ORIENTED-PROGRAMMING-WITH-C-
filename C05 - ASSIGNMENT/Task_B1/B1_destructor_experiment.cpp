// B1.4: compile with -DVIRTUAL_DTOR to get the correct version.
#include <iostream>
struct Bill {
#ifdef VIRTUAL_DTOR
    virtual ~Bill() { std::cout << "~Bill\n"; }
#else
    ~Bill() { std::cout << "~Bill\n"; }
#endif
    virtual double total() const = 0;
};
struct InPatient : Bill {
    double* itemised = new double[1000];                      // heap memory owned by the derived class
    ~InPatient() { delete[] itemised; std::cout << "~InPatient (itemised freed)\n"; }
    double total() const override { return 47500; }
};
int main()
{
    Bill* b = new InPatient;
    std::cout << b->total() << '\n';
    delete b;                                                  // UB if ~Bill is not virtual
}
