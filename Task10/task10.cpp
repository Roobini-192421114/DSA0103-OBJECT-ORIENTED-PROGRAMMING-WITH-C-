#include <iomanip>
#include <iostream>
int main(){for(long long n:{20000LL,500000LL,3000000LL}){long long paise=n*3000;double rupees=static_cast<double>(paise)/100.0;std::cout<<"Journeys "<<n<<" total paise "<<paise<<" total Rs "<<std::fixed<<std::setprecision(2)<<rupees<<"\n";}}