/* Implementation of PLSL Interpreter
 * parseInt.cpp
 * Programming Assignment 3
 * Spring 2022
*/

#include <vector>
#include <string>
#include "parseInt.h"
#include "val.h"
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//#include "lex.cpp"
using namespace std;
map<string, bool> defVar;
map<string, Token> SymTable;
map<string, string> values;

map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constants 
queue <Value> * ValQue; //declare a pointer variable to a queue of Value objects
vector <string> strList;
string stringer;
bool flag;
int globNum;


namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

bool IdentList(istream& in, int& line, vector<string> &IdList);

//WriteStmt:= wi, ExpreList 
bool WriteLnStmt(istream& in, int& line) {
	LexItem t;
	//cout << "in WriteStmt" << endl;
	ValQue = new queue<Value>;
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression after WriteLn");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	
	
	//Evaluate: print out the list of expressions' values
	while (!(*ValQue).empty())
	{
		Value nextVal = (*ValQue).front();
		cout << nextVal;
		ValQue->pop();
	}
	cout << endl;

	return ex;
}//End of WriteLnStmt


//Expr:= Term {(+|-) Term}
bool Expr(istream& in, int& line, Value& retVal) {
	Value val1, val2;
	//cout << "in Expr" << endl;
	bool t1 = Term(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		//cout << "first Term err in EXPR" << endl;
		return false;
	}

	retVal = val1;
	
	tok = Parser::GetNextToken(in, line);
	
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	//Evaluate: evaluate the expression for addition or subtraction
	
	//cout<< tok.GetLexeme() << endl;
	while ( tok == PLUS || tok == MINUS ) 
	{
		t1 = Term(in, line, val2);
		if( !t1 ) 
		{
			//cout<< "error in EXPR with term" << endl;
			ParseError(line, "Missing operand after operator");
			return false;
		}
		
		if(tok == PLUS)
		{
			//cout<< retVal.GetType()<< " " << val2.GetType() << endl;
			retVal = retVal + val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal addition operation.");
				//cout << "(" << tok.GetLexeme() << ")" << endl;		
				return false;
			}
		}
		else if(tok == MINUS)
		{
			retVal = retVal - val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal subtraction operation.");
				return false;
			}
		}
			
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}

	if (tok == GTHAN){
		t1 = SFactor(in, line, val2);
		
		if( !t1 ) 
		{
			//cout<< "error in EXPR with term" << endl;
			ParseError(line, "Missing operand after operator");
			return false;
		}

		//cout<< retVal.GetReal()<< " skrt " << val2.GetType()<< endl;
		retVal = retVal > val2;
		if(retVal.IsErr())
		{
			ParseError(line, "Run-Time Error-Illegal Mixed Type Operands for a Logic Expression.");
			return false;
		}

		flag= retVal.GetBool();
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}	
	}

	if (tok == LTHAN){
		t1 = SFactor(in, line, val2);
		
		if( !t1 ) 
		{
			//cout<< "error in EXPR with term" << endl;
			ParseError(line, "Missing operand after operator");
			return false;
		}

		//cout<< retVal.GetReal()<< " skrt " << val2.GetType()<< endl;
		retVal = retVal < val2;
		if(retVal.IsErr())
		{
			ParseError(line, "Run-Time Error-Illegal Mixed Type Operands for a Logic Expression.");
			return false;
		}

		flag= retVal.GetBool();
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}	
	}
	
	if (tok == EQUAL){

		t1 = SFactor(in, line, val2);
		
		if( !t1 ) 
		{
			//cout<< "error in EXPR with term" << endl;
			ParseError(line, "Missing operand after operator");
			return false;
		}

		//cout<< retVal.GetReal()<< " skrt " << val2.GetType()<< endl;
		retVal = retVal == val2;
		if(retVal.IsErr())
		{
			ParseError(line, "Run-Time Error-Illegal Mixed Type Operands for a Logic Expression.");
			return false;
		}

		flag= retVal.GetBool();
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}	
	}
	//cout << tok.GetLexeme()<<endl;

	Parser::PushBackToken(tok);

	return true;
}//end of Expr


bool IdentList(istream& in, int& line);


//Program is: Prog = PROGRAM IDENT {Decl} {Stmt} END PROGRAM IDENT
bool Prog(istream& in, int& line)
{
	bool f1, f2;
	LexItem tok = Parser::GetNextToken(in, line);
		
	if (tok.GetToken() == PROGRAM) {
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT) {
			
			tok = Parser::GetNextToken(in, line);
			if (tok.GetToken() == SEMICOL) {
				f1 = DeclBlock(in, line); 
			
				if(f1) {
					f2 = ProgBody(in, line);
					if(!f2)
					{
						ParseError(line, "Incorrect Program Body.");
						return false;
					}
					
					return true;//Successful Parsing is completed
				}
				else
				{
					ParseError(line, "Incorrect Declaration Section.");
					return false;
				}
			}
			else
			{
				//Parser::PushBackToken(tok);
				ParseError(line-1, "Missing Semicolon.");
				return false;
			}
		}
		else
		{
			ParseError(line, "Missing Program Name.");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else if(tok.GetToken() == DONE && tok.GetLinenum() <= 1){
		ParseError(line, "Empty File");
		return true;
	}
	ParseError(line, "Missing PROGRAM.");
	return false;
}

bool ProgBody(istream& in, int& line){
	bool status;
		
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok.GetToken() == BEGIN) {
		
		status = Stmt(in, line);
		
		while(status)
		{
			tok = Parser::GetNextToken(in, line);
			if(tok != SEMICOL)
			{
				line--;
				ParseError(line, "Missing semicolon in Statement.");
				return false;
			}
			
			status = Stmt(in, line);
		}
			
		tok = Parser::GetNextToken(in, line);
		if(tok == END )
		{
			return true;
		}
		else 
		{
			ParseError(line, "Syntactic error in Program Body.");
			return false;
		}
	}
	else
	{
		ParseError(line, "Non-recognizable Program Body.");
		return false;
	}	
}//End of ProgBody function

bool DeclBlock(istream& in, int& line) {
	bool status = false;
	LexItem tok;
	//cout << "in Decl" << endl;
	LexItem t = Parser::GetNextToken(in, line);
	if(t == VAR)
	{
		status = DeclStmt(in, line);
		
		while(status)
		{
			tok = Parser::GetNextToken(in, line);
			if(tok != SEMICOL)
			{
				line--;
				ParseError(line, "Missing semicolon in Declaration Statement.");
				return false;
			}
			status = DeclStmt(in, line);
		}
		
		tok = Parser::GetNextToken(in, line);
		if(tok == BEGIN )
		{
			Parser::PushBackToken(tok);
			return true;
		}
		else 
		{
			ParseError(line, "Syntactic error in Declaration Block.");
			return false;
		}
	}
	else
	{
		ParseError(line, "Non-recognizable Declaration Block.");
		return false;
	}
	
}//end of DeclBlock function

bool DeclStmt(istream& in, int& line)
{
	LexItem t;
	bool status = IdentList(in, line);
	
	if (!status)
	{
		ParseError(line, "Incorrect variable in Declaration Statement.");
		return status;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t == COLON)
	{
		t = Parser::GetNextToken(in, line);
		if(t == INTEGER || t == REAL || t == STRING)
		{
			// for (const auto& [key, value]: defVar) {
			// 	if(defVar.find(key) != defVar.end()){
			// 		cout<< key << " "<< t.GetToken() << endl;
			// 		SymTable[key]= t.GetToken();
			// 	}
			// }

			// auto iter = defVar.begin();
    		// while (iter != defVar.end()) {
			// 	int num = defVar.count(iter->first);
			// 	cout<< "hey1" << endl;
			// 	if(num<=0){
			// 		cout<< "hey2" << endl;
			//  		SymTable[iter->first]= t.GetToken();
			//  	}
			// 	iter++;
    		// }
			map<string, bool>:: iterator iter;
			for(iter = defVar.begin(); iter != defVar.end(); iter++){
                    if(SymTable.find(iter->first) == SymTable.end() ){
						//cout<< iter->first << " "<< t.GetToken() << endl;
                        SymTable.insert(pair<string, Token>(iter->first, t.GetToken()));
                    }
                }

			return true;
		}
		else
		{
			ParseError(line, "Incorrect Declaration Type.");
			return false;
		}
	}
	else
	{
		Parser::PushBackToken(t);
		return false;
	}
	
}//End of DeclStmt

//IdList:= IDENT {,IDENT}
bool IdentList(istream& in, int& line) {
	bool status = false;
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok == IDENT)
	{
		//set IDENT lexeme to the type tok value
		identstr = tok.GetLexeme();
		if (!(defVar.find(identstr)->second))
		{
			defVar[identstr] = true;
		}	
		else
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}
		
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	
	tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		status = IdentList(in, line);
	}
	else if(tok == COLON)
	{
		Parser::PushBackToken(tok);
		return true;
	}
	else {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return status;
	
}//End of IdentList
	

//Stmt is either a WriteLnStmt, ForepeatStmt, IfStmt, or AssigStmt
bool Stmt(istream& in, int& line) {
	bool status;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {

	case WRITELN:
		status = WriteLnStmt(in, line);
		
		break;

	case IF:
		status = IfStmt(in, line);
		break;

	case IDENT:
		//cout<< t.GetLexeme() << endl;
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);
		break;
		
	default:
		Parser::PushBackToken(t);
		return false;
	}

	return status;
}//End of Stmt

//IfStmt:= if (Expr) then Stm} [Else Stmt]
bool IfStmt(istream& in, int& line) {
	bool ex=false, status ; 
	LexItem t;

	Value retVal;
	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	//cout << "in PAREN going into Logicexpr" << endl; 
	ex = LogicExpr(in, line, retVal);
	if( !ex ) {
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	
	//cout << "after if going to ELSE" << endl; 
	
	if(flag){
		t = Parser::GetNextToken(in, line);
		if(t != THEN)
		{
			ParseError(line, "If-Stmt Syntax Error");
			return false;
		}
		status = Stmt(in, line);
		
		if(!status)
		{
			ParseError(line, "Missing Statement for If-Stmt Then-Part");
			return false;
		}
		
		while(t.GetToken() != SEMICOL){
			t=Parser::GetNextToken(in, line);
		}

	}
	else if (!flag){
		while(t.GetToken() != ELSE){
			t=Parser::GetNextToken(in, line);
		}
		if( t == ELSE ) {
			status = Stmt(in, line);
			if(!status)
			{
				ParseError(line, "Missing Statement for If-Stmt Else-Part");
				return false;
			}
			//cout << "in IFStmt status of Stmt true" << endl;
			return true;
		}
	}

		
	Parser::PushBackToken(t);
	return true;
}//End of IfStmt function

//Var:= ident
bool Var(istream& in, int& line, LexItem& tok)
{
	//called only from the AssignStmt function
	string identstr;
	
	tok = Parser::GetNextToken(in, line);
	
	if (tok == IDENT){
		identstr = tok.GetLexeme();
		
		if (!(defVar.find(identstr)->second))
		{
			ParseError(line, "Undeclared Variable");
			return false;
		}
		strList.push_back(identstr);
		stringer=identstr;
		return true;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}//End of Var

//AssignStmt:= Var = Expr
bool AssignStmt(istream& in, int& line) {
	Value retVal;
	bool varstatus = false, status = false;
	LexItem t;
	LexItem t2;
	
	varstatus = Var( in, line, t2);
	
	
	if (varstatus){
		t = Parser::GetNextToken(in, line);
		//cout<< "In AssignStmt" << endl;
		if (t == ASSOP){
			status = Expr(in, line, retVal);
			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statment");
				return status;
			}
			
		}
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	
	TempsResults[t2.GetLexeme()] = retVal;
	strList.push_back(t.GetLexeme());
	return status;	
}

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	Value retVal;
	bool status = false;
	
	status = Expr(in, line, retVal);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}

	//cout<< retVal.GetType() << " " << retVal << endl;

	(*ValQue).push(retVal);
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		
		status = ExprList(in, line);
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}

//Term:= SFactor {(*|/) SFactor}
bool Term(istream& in, int& line, Value& retVal) {
	Value val1, val2;
	bool t1 = SFactor(in, line, val1);
	LexItem tok;
	//cout << "in term" << endl;
	if( !t1 ) {
		//cout << "first Sfactor err in TERM" << endl;
		return false;
	}
	
	retVal= val1;

	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	
	//cout<<line << " skrrt "<<tok.GetLexeme() << endl;
	while ( tok == MULT || tok == DIV  )
	{
		
		// if(tok==  DIV){
		// 	//cout<< "in if Div" << endl;
		// 	tok= Parser::GetNextToken(in, line);

		// 	if(values[tok.GetLexeme()]=="0"){
		// 		ParseError(line, "Run-Time Error-Illegal Division by Zero");
		// 		return false;
		// 	}
		// 	else{
		// 		Parser::PushBackToken(tok);
		// 	}
		// }

		//cout<< line << "NEW skrrt "<<tok.GetLexeme() << endl;
		//t1 = SFactor(in, line, retVal);
		t1 = SFactor(in, line, val2);
		//cout<< retVal<< endl;
		
		if( !t1 ) {
			//cout<< "error in Term in while loop" << endl;
			ParseError(line, "Missing operand after operator");
			return false;
		}
		
		if(tok == MULT)
		{
			//cout<< retVal.GetType()<< " " << val2.GetType() << endl;
			retVal = retVal * val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal multiplication operation.");
				//cout << "(" << tok.GetLexeme() << ")" << endl;		
				return false;
			}
		}
		else if(tok == DIV)
		{
			if(val2.GetType()== VINT){
				if(val2.GetInt()==0){
					ParseError(line, "Run-Time Error-Illegal Division by Zero");
					return false;
				}
			}
			if(val2.GetType()== VREAL){
				if(val2.GetReal()==(float)0.00){
					ParseError(line, "Run-Time Error-Illegal Division by Zero");
					return false;
				}
			}

			retVal = retVal / val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal division operation.");
				return false;
			}
		}

		
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}

//SFactor = Sign Factor | Factor
bool SFactor(istream& in, int& line, Value& retVal)
{
	LexItem t = Parser::GetNextToken(in, line);
	//cout<< t.GetLexeme() << endl;
	bool status;
	int sign = 0;
	if(t == MINUS )
	{
		sign = -1;

	}
	else if(t == PLUS)
	{
		sign = 1;
	}
	else
		Parser::PushBackToken(t);
		
	status = Factor(in, line, sign, retVal);
	return status;
}

//LogicExpr = Expr (== | <) Expr
bool LogicExpr(istream& in, int& line, Value & retVal)
{
	Value val1, val2;
	bool t1 = Expr(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	retVal= val1;
	tok = Parser::GetNextToken(in, line);
	
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	//cout<< tok.GetLexeme()<< endl;
	
	if ( tok == GTHAN  || tok == EQUAL  || tok == LTHAN)
	{
		//cout << retVal.GetType() << endl;

		t1 = Expr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing expression after relational operator");
			return false;
		}
		return true;
	}
	Parser::PushBackToken(tok);
	return true;
}

//Factor := ident | iconst | rconst | sconst | (Expr)
bool Factor(istream& in, int& line, int sign, Value& retVal) {
	LexItem tok = Parser::GetNextToken(in, line);
	string lexeme = tok.GetLexeme();
	
	//cout<< "STRING: " << stringer<< " TOKEN: "<< SymTable[stringer] <<" lexeme: " <<lexeme<<" line: " <<line<< endl;

	values[stringer]= lexeme;
	
	//cout<< lexeme<< endl;
	
	if( tok == IDENT ) {
		//cout<< "STRING: " << stringer<< " TOKEN: "<< SymTable[stringer] <<" lexeme: " <<lexeme<<" Tok.getToken: " <<tok.GetToken()<< endl;
		//cout<< "IDENT in Factor with "<< lexeme << endl;
		
		if ((!(defVar.find(lexeme)->second)) || std::find(strList.begin(), strList.end(), tok.GetLexeme()) == strList.end())
		{
			ParseError(line, "Using Undefined Variable");
			return false;
		}

		//if(SymTable.find(lexeme) != SymTable.end() ){
		// if(SymTable[lexeme] == INTEGER){
		// 	cout << SymTable[lexeme] <<" "<< lexeme << endl;
		// 	cout<< retVal.GetType()<< endl;
		// }
		if(sign==-1){
			retVal = TempsResults[lexeme] * -1;
		}
		else{
			retVal = TempsResults[lexeme];
		}
		

		if(SymTable[lexeme]==INTEGER && retVal.IsReal()){
			int val= (int)retVal.GetReal();
			retVal.SetType(VINT);
			retVal.SetInt(val);
			return true;
        }

		//cout<< retVal.GetType() << endl;

		//cout<<" retVal: "<< retVal<< endl;
		return true;
	}
	else if( tok == ICONST ) {
		
		//cout << stringer<< " TOKEN "<< SymTable[stringer] <<" and " <<lexeme<< endl;
		if(SymTable[stringer] == REAL){
			retVal= stof(lexeme);
			return true;
		}

		if(sign==-1){
			lexeme= "-"+lexeme;
		}
		//cout<<lexeme<<endl;
		retVal = stoi(lexeme);
		return true;
	}
	else if( tok == SCONST ) {
		//cout << stringer<< " Yur "<< lexeme << endl;
		if(sign!=0){
			ParseError(line, "Illegal Operand Type for Sign Operator");
			return false;
		}
		
		//cout<< stringer << " "<< SymTable[stringer] << endl;
		if ((SymTable[stringer] != STRING)){
			//cout<< lexeme << endl;
			tok= Parser::GetNextToken(in, line);
			// cout<< lexeme.length()<< endl;
			// cout<< tok.GetToken()<< endl;
			
			if (lexeme.length() >0 && tok.GetToken() != SEMICOL)
			{
				Parser::PushBackToken(tok);
				//cout << "you hit me--" << lexeme << endl;
				retVal = lexeme;
				return true;
			}
			else{
				Parser::PushBackToken(tok);
			}
			//cout<<tok.GetLexeme()<< " "<< stringer <<" "<< SymTable[stringer] << endl; 
			
			ParseError(line, "Illegal Assignment Operation");
			return false;
		}
		retVal = lexeme;
		return true;
	}
	else if( tok == RCONST ) {
		//cout<< stringer << " "<< SymTable[stringer] <<" "<<lexeme <<endl;
		if(SymTable[stringer] == INTEGER){
			retVal= stoi(lexeme);
			return true;
		} 
		retVal = stof(lexeme);
		return true;
	}
	else if( tok == LPAREN ) {
		bool ex = Expr(in, line, retVal);
		if( !ex ) {
			ParseError(line, "Missing expression after (");
			return false;
		}
		if( Parser::GetNextToken(in, line) == RPAREN )
			return ex;
		else 
		{
			Parser::PushBackToken(tok);
			ParseError(line, "Missing ) after expression");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	return false;
}



