#include <array>
#include <cstddef>
#include <iostream>
#include <optional>
enum class Zone{Inner,Middle,Outer,Airport};constexpr std::array<long long,4> BASE={1000,2000,3000,6000};
bool peak(int h){return(h>=8&&h<10)||(h>=17&&h<20);}
std::optional<long long> fare(Zone z,int h,bool weekend){if(h<0||h>23)return std::nullopt;auto i=static_cast<std::size_t>(z);if(i>=BASE.size())return std::nullopt;long long b=BASE[i];if(weekend)return b-b*10/100;if(peak(h))return b+b*20/100;return b;}
class MonthlyPass{Zone a,b;int from,to;public:MonthlyPass(Zone x,Zone y,int f,int t):a(x),b(y),from(f),to(t){}bool covers(Zone z,int day)const{return day>=from&&day<=to&&(z==a||z==b);}};
int main(){auto wday=fare(Zone::Airport,8,false),weekend=fare(Zone::Airport,8,true);std::cout<<"Airport weekday peak paise: "<<*wday<<"\nAirport weekend paise: "<<*weekend<<"\n";MonthlyPass p(Zone::Inner,Zone::Middle,1,30);std::cout<<"Pass covers Inner: "<<std::boolalpha<<p.covers(Zone::Inner,12)<<"\nPass covers Airport: "<<p.covers(Zone::Airport,12)<<"\n";}