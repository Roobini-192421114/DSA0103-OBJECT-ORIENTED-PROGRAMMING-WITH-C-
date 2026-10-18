#include <iostream>
enum class Zone{Inner,Middle,Outer};constexpr long long BASE[]={1000,2000,3000};constexpr long long MIN_BALANCE=5000;
bool peak(int h){return(h>=8&&h<10)||(h>=17&&h<20);}long long fare(Zone z,int h){if(h<0||h>23)return -1;long long b=0;switch(z){case Zone::Inner:b=BASE[0];break;case Zone::Middle:b=BASE[1];break;case Zone::Outer:b=BASE[2];break;default:return -1;}return peak(h)?b+b*20/100:b;}
class SmartCard{long long b;public:explicit SmartCard(long long x):b(x>=0?x:0){}bool travel(long long f){if(f<0||b-f<MIN_BALANCE)return false;b-=f;return true;}long long balance()const{return b;}};
int main(){SmartCard c(10000);auto f=fare(Zone::Outer,17);std::cout<<(c.travel(f)?"Approved":"Denied")<<"; balance paise="<<c.balance()<<"\n";}