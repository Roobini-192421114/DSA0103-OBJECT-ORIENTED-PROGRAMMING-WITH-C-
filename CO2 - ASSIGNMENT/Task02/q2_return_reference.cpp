#include <iostream>
using namespace std;

int& highest(int marks[], int n) {
    int best = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[best]) best = i;
    return marks[best];
}
const int& highestConst(const int marks[], int n) {
    int best = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[best]) best = i;
    return marks[best];
}
int highestCopy(const int marks[], int n) {
    int best = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[best]) best = i;
    return marks[best];
}
// Intentionally unsafe example; do not call/use the returned reference.
// int& dangling() { int local = 42; return local; }

int main() {
    int marks[] = {78, 91, 84, 88};
    cout << "highest before: " << highest(marks, 4) << '\n';
    highest(marks, 4) += 2;
    cout << "highest after : " << highest(marks, 4) << '\n';
    cout << "copy result   : " << highestCopy(marks, 4) << '\n';
    cout << "array         : ";
    for (int m : marks) cout << m << ' ';
    cout << '\n';
}
