#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <iterator>
#include <stdio.h>
#include <ctype.h>
#include "lex.h" //get rid of at the end
using namespace std;
#include <algorithm>


vector<string> findTok { "PROGRAM", "WRITELN", "INTEGER", "BEGIN", "END", "IF", "REAL", "STRING","VAR", "ELSE", "FOR", "THEN", "DO", 
    "TO", "DOWNTO", "IDENT", "ICONST", "RCONST", "SCONST", "PLUS", "MINUS", "MULT", "DIV", "ASSOP", "LPAREN", "RPAREN", "COMMA", 
    "EQUAL", "GTHAN", "LTHAN", "SEMICOL", "COLON", "ERR", "DONE"};

map<string, Token> mapTok {{"PROGRAM", PROGRAM}, {"WRITELN",WRITELN}, {"INTEGER",INTEGER}, {"BEGIN",BEGIN}, {"END",END}, {"IF",IF}, 
    {"REAL",REAL}, {"STRING",STRING},{"VAR",VAR}, {"ELSE",ELSE}, {"FOR",FOR}, {"THEN",THEN}, {"DO",DO}, {"TO",TO}, {"DOWNTO",DOWNTO}, 
    {"IDENT",IDENT}, {"ICONST",ICONST}, {"RCONST",RCONST}, {"SCONST",SCONST}, {"PLUS",PLUS}, {"MINUS",MINUS}, {"MULT",MULT}, {"DIV",DIV}, 
    {"ASSOP",ASSOP}, {"LPAREN",LPAREN}, {"RPAREN",RPAREN}, {"COMMA",COMMA}, {"EQUAL",EQUAL}, {"GTHAN",GTHAN}, {"LTHAN",LTHAN}, 
    {"SEMICOL",SEMICOL}, {"COLON",COLON}, {"ERR",ERR}, {"DONE",DONE}};



LexItem getNextToken(istream& in, int& linenum){
    enum TokState { START, INID, INSTRING, ININT, INREAL, INCOMMENT}
    lexstate = START;
    string lexeme;
    char ch;
    string realNum;

    while(in.get(ch)) {
        string str;

        switch( lexstate ) {
            case START:
                //cout << "start(" << ch <<")"<< endl;
                if( ch == '\n' ){
                    linenum++;
                    continue;
                }

                else if(isspace(ch) || ch=='\t')
                    continue;

                else if( ch == '\'' ) {
                    //cout << "String start" <<endl;
                    lexstate = INSTRING;
                    in.putback(ch);
                    continue;
                }

                else if(isalpha(ch) || ch=='_'){
                    in.putback(ch);
                    //cout << ch << " is letter"<< endl;
                    lexstate = INID;
                    continue;
                }

                else if(isdigit(ch)){ 
                    //cout << "Number Start" << endl;
                    lexstate = ININT;
                    in.putback(ch);
                    continue;
                }

                else if (ispunct(ch)){
                    if (ch == '+'){ 
                        str.append(1,ch);
                        return LexItem(PLUS, str, linenum);}
                    else if(ch == '-'){ 
                        str.append(1,ch);
                        return LexItem(MINUS, str, linenum);}
                    else if(ch == '*'){ 
                        str.append(1,ch);
                        return LexItem(MULT, str, linenum);}
                    else if(ch == '/'){ 
                        str.append(1,ch);
                        return LexItem(DIV, str, linenum);}
                    else if(ch == '('){ 
                        //cout << "in LPAREN" << endl;
                        if (in.peek() == '*'){
                            //cout << "going to comment sect" << endl;
                            in.putback(ch);
                            lexstate = INCOMMENT;
                            continue;
                        }
                        else{
                            str.append(1,ch);
                            return LexItem(LPAREN, str, linenum);
                        }
                        
                    }
                    else if(ch == ')'){ 
                        str.append(1,ch);
                        return LexItem(RPAREN, str, linenum);}
                    else if(ch == ','){ 
                        str.append(1,ch);
                        return LexItem(COMMA, str, linenum);}
                    else if(ch == '='){ 
                        str.append(1,ch);
                        return LexItem(EQUAL, str, linenum);}
                    else if(ch == '>'){ 
                        str.append(1,ch);
                        return LexItem(GTHAN, str, linenum);}
                    else if(ch == '<'){ 
                        str.append(1,ch);
                        return LexItem(LTHAN, str, linenum);}
                    else if(ch == ';'){ 
                        str.append(1,ch);
                        return LexItem(SEMICOL, str, linenum);}
                    else if(ch == ':'){ 
                        if(in.peek() == '='){
                            str.append(1,ch);
                            in.get(ch);
                            str.append(1,ch);
                            return LexItem(ASSOP, str, linenum);
                        }
                        else{
                            str.append(1,ch);
                            return LexItem(COLON, str, linenum);
                        }
                    }
                    else if(ch == '.'){ 
                        if (isdigit(in.peek())){
                            realNum.append(1,ch);
                            in.putback(ch);
                            //cout << "Found deci from dot" << endl;
                            lexstate = INREAL;
                            continue;
                        }
                        else{
                            str.append(1,ch);
                            return LexItem(DOT, str, linenum);
                        }
                    }
                    else{
                        str.append(1,ch);
                        return LexItem(ERR, str, linenum);
                    }
                }

                

            case ININT: //for recognizing integer constants (ICONST)
                //cout << "in INT with " << ch << endl;
                if(isdigit(ch)){
                    realNum==""; //reset realNum
                    realNum.append(1,ch);
                    
                    while (in.get(ch)){
                        if(isdigit(ch)){
                            //str.append(1,ch);
                            realNum.append(1,ch);
                        }
                        else if(ch == '.'){
                            realNum.append(1,ch);
                            //cout << "Going to reals" << endl;
                            in.putback(ch); //TESTING (*(&*YU(IUYIGHBKJH)))
                            lexstate = INREAL;
                            goto cnt;
                        }
                        else if(ch==' ' || ch==',' || isalpha(ch) || ch== '_' || ch == '\n' || ch == ';'){
                            in.putback(ch);
                            lexstate= START;
                            return LexItem(ICONST, realNum , linenum);
                        }
                    }
                }
                else{
                    //cout << "ch letter is "<< ch<< endl;
                    in.putback(ch);
                    lexstate= START;
                    goto cnt;
                }
                cnt:;
                continue;

            case INID: //for recognizing identifiers (IDENT)
                str.append(1,ch);
                while (in.get(ch)){
                    if (isalpha(ch) || ch=='_' || isdigit(ch)){
                        str.append(1,ch);
                    } 
                    else {
                        in.putback(ch);
                        //cout << "word:" << str << ":::"<< endl;
                        return id_or_kw(str, linenum);
                    }
                }


            case INSTRING: //for recognizing string literals (SCONST)
                //cout << "Im in INSTRING" << endl;
                if(ch=='\''){
                    str.append(1,ch);
                    string word="";
                    while(in.get(ch)){
                        if (ch== '\''){
                            return LexItem(SCONST, word, linenum);
                        }
                        else if(ch == ' ' && in.peek()== '\n'){
                            //cout << "Im in ERROR1" << endl;
                            str.append(1,ch);
                            //cout << str<< linenum<< endl;
                            return LexItem(ERR, str, linenum);
                        }
                        else if (ch == '\"'){
                            str.append(1,ch);
                            //cout << str<< linenum<< endl;
                            return LexItem(ERR, str, linenum);
                        }
                        else if (ch!='\''){
                            str.append(1,ch);
                            word.append(1,ch);
                        }
                    }
                }
            case INREAL: 
                // need to use realNum int variable
                //cout << "in real with " << realNum << endl;
                
                if(realNum =="." || realNum == "0."){
                    string deci= "0.";
                    while(in.get(ch)){
                        if(isdigit(ch)){
                            deci.append(1,ch);
                        }
                        else if(ch=='.'){
                            deci.append(1,ch);
                            return LexItem(ERR, deci, linenum);
                        }
                        else if (ch == ' ' || ch=='\n' || ch== ','){
                            in.putback(ch);
                            //cout<< "return real from dot" << endl;
                            return LexItem(RCONST, deci, linenum);
                        }
                    }
                }
                else{
                    //cout<< "in else " << endl;
                    while(in.get(ch)){
                        if(isdigit(ch)){
                            realNum.append(1,ch);
                        }
                        else if(ch=='.'){
                            realNum.append(1,ch);
                            //cout << "found error" << realNum << linenum<<endl;
                            return LexItem(ERR, realNum, linenum);
                        }
                        else if (ch==' ' || ch=='\n'){
                            in.putback(ch);
                            //cout<< "return real from dot" << endl;
                            return LexItem(RCONST, realNum, linenum);
                        }
                    }   
                }

            case INCOMMENT:
                //cout << "in the comment sect" << endl;
                if (ch == '('){
                    if (in.peek() == '*'){
                        in.get(ch);
                        //cout << "got start of comment" << endl;
                    }
                }

                while(in.get(ch)){
                    if( ch == '\n' )
                        linenum++;
                    else if (ch == '*'){
                        if (in.peek()== ')'){
                            //cout << "got end of comment" << endl;
                            in.get(ch);
                            lexstate = START;
                            goto ctn;
                        }
                    }
                    else{
                        continue;
                    }
                }
                return LexItem(ERR, "*)", linenum);
                
                ctn:;
                continue;
        }   

    }
    return LexItem(ERR, "FIX LATER", linenum);
}


//Other states will follow

//peeek() to see next ch

///putback() to .get() the same ch again

LexItem id_or_kw(const string& lexeme, int linenum){
    string capLex= lexeme; 
    transform(capLex.begin(), capLex.end(),capLex.begin(), ::toupper);
    //cout << capLex << endl;

    if(find(findTok.begin(), findTok.end(), lexeme) != findTok.end()){
        return LexItem(mapTok[lexeme], lexeme, linenum);
    }
    else if(find(findTok.begin(), findTok.end(), capLex) != findTok.end()){
        return LexItem(mapTok[capLex], lexeme, linenum);
    }
    else{
        return LexItem(IDENT, capLex, linenum);
    }
}

ostream& operator<<(ostream& out, const LexItem& tok){
    
    out << findTok[tok.GetToken()] << "(" << tok.GetLexeme()<< ")" << endl;

    return out;
}




