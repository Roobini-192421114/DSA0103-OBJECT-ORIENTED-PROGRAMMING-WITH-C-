#include <iostream>
enum class Zone{Inner,Middle,Outer};constexpr long long BASE[]={1000,2000,3000};
bool peak(int h){return(h>=8&&h<10)||(h>=17&&h<20);}
long long fare(Zone z,int h){if(h<0||h>23)return -1;long long b=0;switch(z){case Zone::Inner:b=BASE[0];break;case Zone::Middle:b=BASE[1];break;case Zone::Outer:b=BASE[2];break;default:return -1;}if(peak(h))b+=b*20/100;return b;}
int main(){std::cout<<"Inner 08:00 paise: "<<fare(Zone::Inner,8)<<"\nOuter 20:00 paise: "<<fare(Zone::Outer,20)<<"\n";}