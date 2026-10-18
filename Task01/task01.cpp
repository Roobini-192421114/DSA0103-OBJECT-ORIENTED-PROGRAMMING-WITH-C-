#include <iostream>
enum class Zone { Inner, Middle, Outer };
class FareTable { public: long long base(Zone z) const { switch(z){case Zone::Inner:return 1000;case Zone::Middle:return 2000;case Zone::Outer:return 3000;} return -1; } };
class SmartCard { long long balance; public: explicit SmartCard(long long b):balance(b>=0?b:0){} bool canTravel(long long f)const{return f>=0&&balance-f>=5000;} bool deduct(long long f){if(!canTravel(f))return false;balance-=f;return true;} long long getBalance()const{return balance;} };
int main(){FareTable f;SmartCard c(10000);auto fare=f.base(Zone::Inner);std::cout<<"Fare paise: "<<fare<<"\nApproved: "<<std::boolalpha<<c.deduct(fare)<<"\nBalance paise: "<<c.getBalance()<<"\n";}