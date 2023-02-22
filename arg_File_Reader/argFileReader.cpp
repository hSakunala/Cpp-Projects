/*
Author: Hrishikesh Sakunala
Date: 1/31/2022
Project Name: RA3_Hrishikesh_Sakunala.cpp
Description: accepts one command line argument for a file name, 
            and ignores any extra command line arguments that may be provided after the first one.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

vector<string> stringToList(string s)
{
    vector<string> a;
    stringstream ss(s);
    string word;
    while (ss >> word) {
        a.push_back(word);
    }
    return(a);
}

int main(int argc, char** argv){

    
    if (argc!=2){
        cout<< "No file name is found" << endl;
    }
    else {
        ifstream myFile;
        string filename= argv[1];
        myFile.open(filename);

        int total=0;
        int nonBlank=0;
        int numWord=0;
        int numInt=0;
        int numName=0;

        if (!myFile.is_open())
        {
            cout<< "File cannot be opened: " << filename << endl;
        }
        else{
            string line;
            while (getline(myFile, line))
            {
                total++;
                if (!line.empty())
                {
                    nonBlank++;
                    vector<string> sepLine = stringToList(line);
                    numWord= numWord + sepLine.size();
                    for (int i=0; i != sepLine.size(); i++){
                        int y=0;
                        
                        
                        if (isdigit(sepLine[i][0])) {
                            if (!(sepLine[i].find(".") != string::npos)){
                                if (!(sepLine[i].find(",") != string::npos)){
                                    if (!(sepLine[i].find("-") != string::npos)){
                                        if (!(sepLine[i].find("_") != string::npos)){
                                            for (int z=0; z!= sepLine[i].length();z++){
                                                if (!(isalpha(sepLine[i][z]))){
                                                    y++;
                                                }
                                            }   
                                            if (y == int(sepLine[i].length())){
                                                numInt++;
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        if (isalpha(sepLine[i][0])){
                            if (!(sepLine[i].find("-") != string::npos)){
                                numName++;
                            }
                        }
                    }

                }    
            }
            if (total==0)
            {
                cout<< "File is empty." <<endl;
            }
            else{
                cout<< "Total Number of Lines: " << total<< endl;
                cout<< "Number of non-blank lines: " << nonBlank<< endl;
                cout<< "Number of Words: " << numWord<< endl;
                cout<< "Number of Integers: " << numInt << endl;
                cout<< "Number of Names: " << numName<< endl;
            }
        }
    }
}

