#include <iostream>
#include <string>
#include <vector> 
using namespace std; 

#include "val.h"

Value Value::operator+(const Value& op) const{
    Value val;
    if (this->IsString()){
        val.SetType(VSTRING);
        val.SetString(this->GetString());
        if(op.IsString()){
            val.Stemp= val.GetString() + op.GetString();
            return val;
        }
        else{
            val.SetType(VERR);
            return val;
        }
    }
    else if (this->IsInt()){
        val.SetType(VINT);
        val.SetInt(this->GetInt());
        if(op.IsInt()){
            val.Itemp= val.GetInt() + op.GetInt();
            return val;
        }
        if(op.IsReal()){
            val.SetType(VREAL);
            val.SetReal(this->GetInt());
            val.Rtemp= val.GetReal() + op.GetReal();
            return val;
        }
        else{
            val.SetType(VERR);
            return val;
        }
    }
    else if (this->IsReal()){
        val.SetType(VREAL);
        val.SetReal(this->GetReal());
        if(op.IsInt()){
            val.Rtemp= val.GetReal() + op.GetInt();
            return val;
        }
        if(op.IsReal()){
            val.Rtemp= val.GetReal() + op.GetReal();
            return val;
        }
        else{
            val.SetType(VERR);
            return val;
        }
    }
    else{
        val.SetType(VERR);
        return val;
    }
    cout<< "out of bounds" << endl;
    return val;
}

// multiply this by op
Value Value::operator*(const Value& op) const{
    Value val;
    if (this->IsInt()){
        val.SetType(VINT);
        val.SetInt(this->GetInt());
        if(op.IsInt()){
            val.Itemp= val.GetInt() * op.GetInt();
            return val;
        }
        if(op.IsReal()){
            val.SetType(VREAL);
            val.SetReal(this->GetInt());
            val.Rtemp= val.GetReal() * op.GetReal();
            return val;
        }
        else{
            val.SetType(VERR);
            return val;
        }
    }
    
    else if (this->IsReal()){
        val.SetType(VREAL);
        val.SetReal(this->GetReal());
        if(op.IsInt()){
            val.Rtemp= val.GetReal() * op.GetInt();
            return val;
        }
        if(op.IsReal()){
            val.Rtemp= val.GetReal() * op.GetReal();
            return val;
        }
        else{
            val.SetType(VERR);
            return val;
        }
    }
    else{
        val.SetType(VERR);
        return val;
    }
    cout<< "out of bounds" << endl;
    return val;
}

// divide this by op
Value Value::operator==(const Value& op) const{
    Value val;
    if( this->IsBool() || op.IsBool()){
        val.SetType(VERR);
        return val;
    }
    else if (this->IsString()){
        val.SetType(VSTRING);
        val.SetString(this->GetString());
        if(op.IsString()){
            if(val.GetString() == op.GetString()){
                val.SetType(VBOOL);
                val.SetBool(true);
                return val;
            }
            else{
                val.SetType(VBOOL);
                val.SetBool(false);
                return val;
            }
        }
        else{
            val.SetType(VERR);
            return val;
        }
    }

    else if(this->IsInt()){
        val.SetType(VINT);
        val.SetInt(this->GetInt());
        if(op.IsInt()){
            if(val.GetInt() == op.GetInt()){
                val.SetType(VBOOL);
                val.SetBool(true);
                return val;
            }
            else{
                val.SetType(VBOOL);
                val.SetBool(false);
                return val;
            }
        }
        else if(op.IsReal()){
            if(val.GetInt() == op.GetReal()){
                val.SetType(VBOOL);
                val.SetBool(true);
                return val;
            }
            else{
                val.SetType(VBOOL);
                val.SetBool(false);
                return val;
            }
        }
        else{
            val.SetType(VERR);
            return val;
        }
    }
    else if (this->IsReal()){
        val.SetType(VREAL);
        val.SetReal(this->GetReal());
        if(op.IsInt()){
            if (val.GetReal() == op.GetInt()){
                val.SetType(VBOOL);
                val.SetBool(true);
                return val;
            }
            else{
                val.SetType(VBOOL);
                val.SetBool(false);
                return val;
            }
        }
        else if(op.IsReal()){
            if(val.GetReal() == op.GetReal()){
                val.SetType(VBOOL);
                val.SetBool(true);
                return val;
            }
            else{
                val.SetType(VBOOL);
                val.SetBool(false);
                return val;
            }
        }
        else{
            val.SetType(VERR);
            return val;
        }
    }
    else{
        val.SetType(VERR);
        return val;
    }
    cout<< "out of bounds" << endl;
    return val;
}