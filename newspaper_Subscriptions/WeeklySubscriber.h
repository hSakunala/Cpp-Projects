#include <string> 
#include <sstream>
#include <iostream> 
#include "Address.h"

using std::string;
using std::stringstream;

class WeeklySubscriber : public NewsPaperSubscriber
{
    public:
    WeeklySubscriber(Address addy){
        setRate(10.50);
        setAddress(addy);  
    }
    
    void print(){
        std::cout << getAddress().AddrContents()<< std::endl;
        std::cout<< "Rate: "<< getRate()<< std::endl;
        std::cout<< "Service Type: Weekly Subscriber"<< std::endl;
    }
};