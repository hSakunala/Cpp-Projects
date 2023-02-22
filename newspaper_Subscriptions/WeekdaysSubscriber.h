#include <string> 
#include <sstream>
#include <iostream> 
#include "Address.h"

using std::string;
using std::stringstream;

class WeekdaysSubscriber : public NewsPaperSubscriber
{
    public:

    WeekdaysSubscriber(Address addy){
        setRate(7.5);
        setAddress(addy); 
    }

    void print(){
        std::cout << getAddress().AddrContents()<< std::endl;
        std::cout<< "Rate: "<< getRate()<< std::endl;
        std::cout<< "Service Type: Week Days Subscriber"<< std::endl;
    }
};
