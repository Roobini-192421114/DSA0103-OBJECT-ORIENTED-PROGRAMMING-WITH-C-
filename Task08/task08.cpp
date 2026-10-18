#include <iomanip>
#include <iostream>
#include <limits>
int main(){int h;while(true){std::cout<<"Enter hour (0-23): ";if(std::cin>>h){if(h>=0&&h<=23)break;std::cout<<"Out of range.\n";}else{if(std::cin.eof())return 1;std::cout<<"Enter a number.\n";std::cin.clear();std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');}}long long f=(h>=8&&h<10)?1200:1000;std::cout<<"Fare Rs "<<f/100<<'.'<<std::setw(2)<<std::setfill('0')<<f%100<<std::setfill(' ')<<"\n";}