/*
Author: Hrishikesh Sakunala
Date: 3/6/2022
Project Name: PA1_Hrishikesh_Sakunala.cpp
Description: a lexical analyzer for small programming language and a program to test it 
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <iterator>
#include <stdio.h>
#include <ctype.h>
#include "lex.h"  //get rid of at the end
#include <vector>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv){

    vector<string> argue;
    vector<string> modi = {"-v", "-iconst", "-rconst" , "-sconst" , "-ident"};
    int fileNum =0 ;
    string filename;


    bool vFlag = false;
	bool iconstFlag = false;
    bool rconstFlag = false;
	bool sconstFlag = false;
    bool identFlag = false;
    ifstream file;

    for (int i =1; i!=argc; i++){
        if(isalpha(argv[i][0])){
            //cout << argv[i] <<endl;
            filename= argv[i];
            fileNum++;
        }
    }

	
	if (argc == 1)
	{
		cerr << "NO SPECIFIED INPUT FILE NAME." << endl;
		return 0;
	}
	for( int i=1; i<argc; i++ ) {
		string arg( argv[i] );
		if( arg == "-v" )
			vFlag = true;
		else if( arg == "-iconst" )
			iconstFlag = true;
        else if( arg == "-rconst" )
			rconstFlag = true;
        else if( arg == "-sconst" )
			sconstFlag = true;
        else if( arg == "-ident" )
			identFlag = true;
		else if( arg[0] == '-' ) {
			cerr << "UNRECOGNIZED FLAG " << arg << endl;
			return 0;
		}
		else {
			file.open(arg);
			if( file.is_open() == false ) {
				cerr << "CANNOT OPEN the File " << arg << endl;
				return 0;
			}
		}
	}

    //cout<< fileNum << endl;
    if (fileNum!=1){
         
        cout<< "ONLY ONE FILE NAME ALLOWED." << endl;
    }
    else{
        //string flag= argv[2]; //modiefer for diff information
        
        LexItem tok;
        int lineNumber=0;
        string line; 

        if (file.peek() == ifstream::traits_type::eof()){
            cout<< "Lines: 0" << endl; 
        }
        else{
            vector<string> findTok { "PROGRAM", "WRITELN", "INTEGER", "BEGIN", "END", "IF", "REAL", "STRING","VAR", "ELSE", "FOR", "THEN", "DO", 
                "TO", "DOWNTO", "IDENT", "ICONST", "RCONST", "SCONST", "PLUS", "MINUS", "MULT", "DIV", "ASSOP", "LPAREN", "RPAREN", "COMMA", 
                "EQUAL", "GTHAN", "LTHAN", "SEMICOL", "COLON", "ERR", "DONE"};
            string line;
            
            vector<LexItem> items;
            vector<string> sentences;
            vector<int> integers;
            vector<double> doubles;
            vector<string> identit; 

            while((tok = getNextToken(file, lineNumber)) != DONE) {
               items.push_back(tok);
               //cout << tok<< endl;
               if (tok.GetToken() == ERR){
                   break;
               }
            }

            int total= items.size();
            int linePrint =0;
            
            for (int i=0; i< items.size(); i++){
                if(items[i].GetToken()== SCONST){
                    sentences.push_back(items[i].GetLexeme());
                }
                if(items[i].GetToken()== ICONST){
                    stringstream num(items[i].GetLexeme());
                    int a =0;
                    num >> a;
                    integers.push_back(a);
                }
                if(items[i].GetToken()== RCONST){
                    double numD = stod(items[i].GetLexeme());
                    doubles.push_back(numD);
                }
                if(items[i].GetToken()== IDENT){
                    identit.push_back(items[i].GetLexeme());
                }
                if (items[i].GetToken()== ERR)
                    total--;
            }

            if (vFlag){
                for (int i=0 ; i< items.size(); i++){
                    if (items[i].GetToken() == IDENT || items[i].GetToken() == RCONST || items[i].GetToken() == SCONST || items[i].GetToken() == ICONST){
                        cout << items[i];
                    }
                    else if (items[i].GetToken()== ERR){
                        if(items[i].GetLexeme() == "*)"){
                            cout<< "\nMissing a comment end delimiters '*)' at line " << lineNumber<< endl;
                        }
                        else if(ispunct(items[i].GetLexeme()[0])){
                            cout<< "Error in line 2 (" <<items[i].GetLexeme()[0] <<")"<< endl;
                            return 0;
                        }

                    }
                    else{
                        cout<< findTok[items[i].GetToken()] << endl;
                    }
                }
                if (linePrint==0){
                    linePrint++;
                    cout << "Lines: " << lineNumber <<endl;
                    cout << "Tokens: " << total<<endl;
                }
            }

            if (sconstFlag){
                for(int i=0; i< items.size(); i++){
                    if (items[i].GetToken()== ERR && items[i].GetLexeme() != "FIX LATER"){
                        cout << "Error in line "<< lineNumber+1 <<" (" << items[i].GetLexeme() << ")" << endl;
                        return 0;
                    }
                }
                if (linePrint==0){
                    linePrint++;
                    cout << "Lines: " << lineNumber <<endl;
                    cout << "Tokens: " << total<<endl;
                }

                if (sentences.size()>0){
                    cout << "STRINGS:" << endl;
                    sort(sentences.begin(), sentences.end());
                    for (int i=0; i< sentences.size(); i++){
                        cout<< '\'' << sentences[i] << '\''<<endl;
                    }
                }
            }

            if (iconstFlag){
                if (linePrint==0){
                    linePrint++;
                    cout << "Lines: " << lineNumber <<endl;
                    cout << "Tokens: " << total<<endl;
                }

                if (integers.size()>0){
                    cout << "INTEGERS:" << endl;
                    sort(integers.begin(), integers.end());
                    integers.erase( unique( integers.begin(), integers.end() ), integers.end());
                    for (int i=0; i< integers.size(); i++){
                        cout<<  integers[i] <<endl;
                    }
                }
                 
            }

            if (rconstFlag){
                for(int i=0; i< items.size(); i++){
                    if (items[i].GetToken()== ERR && items[i].GetLexeme() != "FIX LATER"){
                        cout << "Error in line "<< lineNumber+1 <<" (" << items[i].GetLexeme() << ")" << endl;
                        return 0;
                    }
                }
                if (linePrint==0){
                    linePrint++;
                    cout << "Lines: " << lineNumber <<endl;
                    cout << "Tokens: " << total<<endl;
                }

                if (doubles.size()>0){
                    cout << "REALS:" << endl;
                    sort(doubles.begin(), doubles.end());
                    doubles.erase( unique( doubles.begin(), doubles.end() ), doubles.end() );
                    for (int i=0; i< doubles.size(); i++){
                        cout<<  doubles[i] <<endl;
                    }
                }
            }
            
            if (identFlag){
                
                if (identit.size()>0){
                    if (linePrint==0){
                        linePrint++;
                        cout << "Lines: " << lineNumber <<endl;
                        cout << "Tokens: " << total<<endl;
                    }
                    
                    cout << "IDENTIFIERS:" << endl;
                    sort(identit.begin(), identit.end());
                    identit.erase( unique( identit.begin(), identit.end() ), identit.end());
                    for (int i=0; i< identit.size(); i++){
                        if(i== identit.size()-1){
                            cout<<  identit[i] << endl;
                        }
                        else{
                            cout<<  identit[i] <<", ";
                        }
                    }
                }
            }



        }
    }
}

