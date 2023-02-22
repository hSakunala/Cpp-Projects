
#include <string> 
#include <sstream> 
#include "Address.h"
#include <iostream> 

using std::string;
using std::stringstream;

class NewsPaperSubscriber
{
    public:
	Address st_Address; 
    double rate;

    NewsPaperSubscriber(){}

    NewsPaperSubscriber(Address addy){
        st_Address= addy;
    }
    
    Address getAddress(){
        return st_Address;
    }

    void setAddress(Address newAddy){
        st_Address= newAddy;
    }

    void setRate(double num){
        rate = num;
    }

    double getRate(){
        return rate; 
    }

    virtual void print()=0;

    bool equals(NewsPaperSubscriber* obj){
        if(st_Address == (*obj).getAddress()){
            return true;
        }
        return false;
    }
};
