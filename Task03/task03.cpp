#include <climits>
#include <iostream>
int main(){long long fare=3000,n=3000000,total=fare*n;double rupees=static_cast<double>(fare)/100.0*n;std::cout<<"sizeof(int): "<<sizeof(int)<<"\nsizeof(long long): "<<sizeof(long long)<<"\nINT_MAX: "<<INT_MAX<<"\nTotal paise: "<<total<<"\nApprox total rupees: "<<rupees<<"\n";}