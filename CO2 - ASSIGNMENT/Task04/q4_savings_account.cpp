#include <iostream>
#include <string>
using namespace std;

class SavingsAccount {
private:
    string accountNumber;
    long long balance;
    long long transactions[10];
    int count;
    void record(long long amount);
public:
    SavingsAccount(const string& number, long long openingBalance)
        : accountNumber(number), balance(openingBalance), transactions{}, count(0) {
        record(openingBalance);
    }
    void deposit(long long amount);
    bool withdraw(long long amount);
    void miniStatement() const;
};
void SavingsAccount::record(long long amount) {
    if (count < 10) transactions[count++] = amount;
    else {
        for (int i = 1; i < 10; ++i) transactions[i - 1] = transactions[i];
        transactions[9] = amount;
    }
}
void SavingsAccount::deposit(long long amount) {
    if (amount <= 0) { cout << "Deposit rejected\n"; return; }
    balance += amount;
    record(amount);
}
bool SavingsAccount::withdraw(long long amount) {
    if (amount <= 0 || amount > balance) { cout << "Withdrawal rejected\n"; return false; }
    balance -= amount;
    record(-amount);
    return true;
}
void SavingsAccount::miniStatement() const {
    cout << "Account: " << accountNumber << "\nBalance: " << balance
         << "\nRecent transactions (positive=deposit/opening, negative=withdrawal):\n";
    for (int i = 0; i < count; ++i) cout << transactions[i] << '\n';
}
int main() {
    SavingsAccount account("SA1001", 1000);
    account.deposit(200); account.withdraw(150); account.deposit(500);
    for (int i = 1; i <= 8; ++i) account.deposit(i * 10);
    account.miniStatement();
}
