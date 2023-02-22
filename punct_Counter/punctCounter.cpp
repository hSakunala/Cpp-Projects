/*
Author: Hrishikesh Sakunala
Date: 1/31/2022
Project Name: RA4_Hrishikesh_Sakunala.cpp
Description: a C++ program that should read from a file name specified in the command line as an argument and counts punctuation
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <iterator>
#include <stdio.h>
#include <ctype.h>
using namespace std;


int main(int argc, char** argv){

    
    if (argc!=2){
        cout<< "No file is found." << endl;
    }
    else {
        ifstream myFile;
        string filename= argv[1];
        myFile.open(filename);

        map<char, int> pMap;
        int total=0;
        

        if (!myFile.is_open())
        {
            cout<< "File cannot be opened: " << filename << endl;
        }
        else{
            string line;
            while (getline(myFile, line)) {
                
                if (!line.empty()) {

                    stringstream ss(line);
                    string word;
                    while (ss >> word) {
                        
                        for (int i=0; i != word.size(); i++){
                            total++;
                            if (ispunct(word[i])){
                                if(pMap.find(word[i]) == pMap.end() ){
                                    pMap.insert(pair<char, int>(word[i], 1));
                                }
                                else{
                                    pMap[word[i]]++;
                                }
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
                int z =0;
                char c;
                cout<< "Number of characters in the file: " << total<< endl;
                cout<< "Punctuation characters in the file and their occurrences:\n" << endl;
                map<char, int>:: iterator iter;
                for(iter = pMap.begin(); iter != pMap.end(); iter++){
                    cout << iter->first << ": " << iter->second << endl;
                    if (iter->second > z){
                        z= iter->second;
                        c= iter->first;
                    }
                }
                cout<< "\nThe punctuation character with maximum occurrences is \""<< c <<"\" found "<< z <<" times." << endl;
            }
        }
    }
}

