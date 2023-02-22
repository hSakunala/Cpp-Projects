/*
Author: Hrishikesh Sakunala
Date: 1/31/2022
Project Name: RA2_Hrishikesh_Sakunala.cpp
Description: reads lines from a file until the end of file
*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;


int main(){
    ifstream myFile;
    string filename;
    
    cout<< "Enter the name of a file to read from: ";
    cin>> filename;
    cout << endl<< endl;
    
    myFile.open(filename);

    int total=0;
    int comment=0;
    int command=0;
    int bold=0;
    int italic=0;
    int reg=0;


    if (!myFile.is_open())
    {
        cout<< "File cannot be opened " << filename << endl;
    }else{
        string line;
        while (getline(myFile, line))
        {
            total++;
            if (line[0] == '%')
            {
                command++;
                if (line.find("bold")!= string::npos)
                {
                    bold++;
                }
                else if (line.find("italic")!= string::npos)
                {
                    italic++;
                }
                else if (line.find("regular")!= string::npos){
                    reg++;
                }
                else{
                    cout << "Error: Unrecognizable command in line " << total << endl << endl;
                }
                
                
            }
            if(line[0]  == '#'){
                comment++;
            }
        }
        myFile.close();
        
        cout<< "Total lines: " << total<< endl;
        cout<< "Commented lines: " << comment<< endl;
        cout<< "Command lines: " << command<< endl;
        cout<< "Bold commands: " << bold<< endl;
        cout<< "Italic commands: " << italic<< endl;
        cout<< "Regular commands: " << reg<< endl;
    }

    
}