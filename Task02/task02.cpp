#include <iostream>
enum class Zone { Inner, Middle, Outer };
constexpr long long MIN_BALANCE_PAISE=5000; constexpr int PEAK_PERCENT=20; constexpr long long BASE_PAISE[]={1000,2000,3000};
int main(){Zone z=Zone::Middle;auto fare=BASE_PAISE[static_cast<int>(z)];std::cout<<"Fare paise: "<<fare<<"\nMinimum balance: "<<MIN_BALANCE_PAISE<<"\nPeak percent: "<<PEAK_PERCENT<<"\n";}