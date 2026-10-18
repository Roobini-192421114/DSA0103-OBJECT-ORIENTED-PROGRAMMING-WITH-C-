#include <iostream>
bool canTravel(long long b,long long f){return f>=0&&b-f>=5000;}bool deduct(long long& b,long long f){if(!canTravel(b,f))return false;b-=f;return true;}
class SmartCard{long long b;public:explicit SmartCard(long long x):b(x){}bool deductFare(long long f){if(f<0||b-f<5000)return false;b-=f;return true;}long long balance()const{return b;}};
int main(){long long b=10000;std::cout<<"Procedural: "<<std::boolalpha<<deduct(b,1000)<<" balance="<<b<<"\n";SmartCard c(10000);std::cout<<"OOP: "<<c.deductFare(1000)<<" balance="<<c.balance()<<"\n";}