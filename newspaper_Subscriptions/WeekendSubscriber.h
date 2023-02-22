#include <string> 
#include <sstream>
#include <iostream> 
#include "Address.h"

using std::string;
using std::stringstream;

class WeekendSubscriber : public NewsPaperSubscriber
{
    public:

    WeekendSubscriber(Address addy){
        setRate(4.50);
        setAddress(addy);  
    }
    
    
    void print(){
        std::cout << getAddress().AddrContents()<< std::endl;
        std::cout<< "Rate: "<< getRate()<< std::endl;
        std::cout<< "Service Type: Weekend Subscriber"<< std::endl;
    }
};
