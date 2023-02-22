/* Definitions and some functions implementations
 * parse.cpp to be completed
 * Programming Assignment 2
 * Spring 2022
 */

#include "parse.h"
#include <vector>
#include <algorithm>

map<string, bool> defVar;
map<string, Token> SymTable;
vector<string> lexems;



namespace Parser
{
	bool pushed_back = false;
	LexItem pushed_token;

	static LexItem GetNextToken(istream &in, int &line)
	{
		if (pushed_back)
		{
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem &t)
	{
		if (pushed_back)
		{
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


bool Stmt(istream &in, int &line)
{
	bool status;
	// cout << "in ContrlStmt" << endl;
	LexItem t = Parser::GetNextToken(in, line);
	switch (t.GetToken())
	{

	case WRITELN:
		status = WriteLnStmt(in, line);
		// cout << "After WriteStmet status: " << (status? true:false) <<endl;
		break;

	case IF:
		status = IfStmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(t);
		status = AssignStmt(in, line);

		break;

	case FOR:
		status = ForStmt(in, line);

		break;

	default:
		cout<< t.GetLexeme()<< endl;
		Parser::PushBackToken(t);
		return false;
	}

	return status;
} // End of Stmt

bool WriteLnStmt(istream &in, int &line) // WriteLnStmt ::= WRITELN (ExprList)
{
	LexItem t;
	// cout << "in WriteStmt" << endl;

	t = Parser::GetNextToken(in, line);
	if (t != LPAREN)
	{
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}

	bool ex = ExprList(in, line);

	if (!ex)
	{
		ParseError(line, "Missing expression after WriteLn");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != RPAREN)
	{

		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	// Evaluate: print out the list of expressions values

	return ex;
}

bool ExprList(istream &in, int &line) // ExprList:= Expr {,Expr}
{
	bool status = false;
	// cout << "in ExprList and before calling Expr" << endl;
	status = Expr(in, line);
	if (!status)
	{
		ParseError(line, "Missing Expression");
		return false;
	}

	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == COMMA)
	{
		// cout << "before calling ExprList" << endl;
		status = ExprList(in, line);
		// cout << "after calling ExprList" << endl;
	}
	else if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


bool Prog(istream &in, int &line)
{
	LexItem tok = Parser::GetNextToken(in, line); 
	if(tok != PROGRAM){
		ParseError(line, "Missing PROGRAM.");
		return false;
	}
	
	tok = Parser::GetNextToken(in, line); 
	if(tok != IDENT){
		ParseError(line, "Missing Program Name."); 
		return false;
	}
	
	tok = Parser::GetNextToken(in, line); 
	if(tok != SEMICOL){
		cout <<"Hey Prog 3" << endl; 
		return false;
	}
	
	bool t1= DeclBlock(in, line);
	if(!t1){
		ParseError(line, "Incorrect Declaration Section."); 
		return false;
	}

	lexems.clear();
	bool t2= ProgBody(in, line);
	if(!t2){
		ParseError(line, "Incorrect Program Body."); 
		return false;
	}
	return true;
}

bool DeclBlock(istream &in, int &line)
{
	LexItem tok = Parser::GetNextToken(in, line); 
	if(tok != VAR){
		cout <<"Hey DeclBlock 1" << endl; 
		return false;
	}
	while (true)
	{
		bool t1= DeclStmt(in,line);
		if(!t1){
			ParseError(line, "Syntactic error in Declaration Block.");
			return false;
		}
		
		LexItem tok = Parser::GetNextToken(in, line); 
		if(tok != SEMICOL){
			cout <<"Hey DeclBlock 3" << endl; 
			return false;
		}
		
		tok = Parser::GetNextToken(in, line); 
		if(tok == BEGIN){
			Parser::PushBackToken(tok);
			return true;
		}
		else{
			Parser::PushBackToken(tok);
		}
	}
}

bool mapWork(istream &in, int &line){
	LexItem tok = Parser::GetNextToken(in, line);
	if(defVar.find(tok.GetLexeme()) == defVar.end()){
		defVar[tok.GetLexeme()]=true;
		return true;
	}
	else{ // it is already in there
		ParseError(line, "Variable Redefinition"); 
		return false;
	}
}

bool DeclStmt(istream &in, int &line)
{
	
	// LexItem tok = Parser::GetNextToken(in, line); 
	// if(std::find(lexems.begin(), lexems.end(), tok.GetLexeme()) != lexems.end() ){
	// 	ParseError(line, "Variable Redefinition"); 
	// 	return false;
	// }
	// else{
	// 	lexems.push_back(tok.GetLexeme());
	// }

	bool t1= mapWork(in, line);
	LexItem tok;
	if(!t1){
		ParseError(line, "Incorrect variable in Declaration Statement."); 
		return false;
	}


	while (true){
		tok = Parser::GetNextToken(in, line);
		if (tok != COMMA){
			return false;
		}

		bool t2= mapWork(in, line);
		if(!t2){
			ParseError(line, "Incorrect variable in Declaration Statement."); 
			return false;
		}

		// LexItem tok = Parser::GetNextToken(in, line);
		// if(std::find(lexems.begin(), lexems.end(), tok.GetLexeme()) != lexems.end() ){
		// 	ParseError(line, "Variable Redefinition"); 
		// 	return false;
		// }
		// else{
		// 	lexems.push_back(tok.GetLexeme());
		// }
		
		tok = Parser::GetNextToken(in, line); 
		if(tok == COLON){
			break;
		}
		else{
			Parser::PushBackToken(tok);
		}
	}

	tok = Parser::GetNextToken(in, line); 
	if(tok != INTEGER && tok != REAL && tok != STRING){
		ParseError(line, "Incorrect Declaration Type.");
		return false;
	}
	return true;
}

bool ProgBody(istream &in, int &line) // MAYBE DONE???
{
	//cout << "In progbody" << endl;
	LexItem tok = Parser::GetNextToken(in, line); 
	if(tok != BEGIN){
		ParseError(line, "No BEGIN");
		return false;
	}
	
	while (true)
	{
		bool t1= Stmt(in,line);
		if(!t1){
			ParseError(line, "Syntactic error in Program Body.");
			return false;
		}
		
		tok = Parser::GetNextToken(in, line); 
		if(tok != SEMICOL){
			line--;
			ParseError(line, "Missing semicolon in Statement.");
			return false;
		}
		
		tok = Parser::GetNextToken(in, line); 
		if(tok != END){
			Parser::PushBackToken(tok);
		}
		else{
			break;
		}
	}
	return true; 
}

bool IfStmt(istream &in, int &line) // IfStmt ::= IF ( LogicExpr ) THEN Stmt [ELSE Stmt]
{
	//cout << "In ifstmt" << endl;
	LexItem tok = Parser::GetNextToken(in, line);
	// cout<< "you hit if stmt" << endl;
	// cout << tok.GetLexeme() << endl; 
	if (tok != LPAREN){
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}

	bool ex = LogicExpr(in, line);
	if (!ex)
	{
		ParseError(line, "Missing LogicExpr");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != RPAREN)
	{
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != THEN)
	{
		ParseError(line, "Missing then statement");
		return false;
	}

	bool t1= Stmt(in, line);
	if(!t1){
		ParseError(line, "Missing Statement for If-Stmt Then-Part");
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok != ELSE){
		Parser::PushBackToken(tok);
	}
	else{
		bool t2 = Stmt(in, line);
		if(!t2){
			ParseError(line, "Error DO in IfStmt");
			return false;
		}
	}
	return true;
}

bool ForStmt(istream &in, int &line)
{
	LexItem tok;

	bool t1 = Var(in, line);
	if(!t1){
		ParseError(line, "Error Var in ForStmt");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != ASSOP)
	{
		ParseError(line, "Need Assop");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != ICONST)
	{
		ParseError(line, "Error ICONST in ForStmt");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != TO && tok != DOWNTO)
	{
		ParseError(line, "Error TO DOWNTO in ForStmt");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != ICONST)
	{
		ParseError(line, "Error ICONST in ForStmt");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != DO)
	{
		ParseError(line, "Error DO in ForStmt");
		return false;
	}

	bool t2= Stmt(in, line);
	if (!t2){
		ParseError(line, "Error Stmt in ForStmt");
		return false;
	} 
	return true;
}

bool AssignStmt(istream &in, int &line)
{
	//cout << "In assignstmt" << endl;
	bool t1= Var(in,line);
	if(!t1){
		ParseError(line, "Error Var in AssignStmt");
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok != ASSOP)
	{
		ParseError(line, "Need Assop in AssignStmt");
		return false;
	}

	bool t2= Expr(in,line);
	if(!t2){
		ParseError(line, "Missing Expression in Assignment Statment");
		return false;
	}
	return true;
}

bool Expr(istream &in, int &line) //Expr ::= Term {(+|-) Term}
{
	//cout << "In progbody" << endl;
	bool t1 = Term(in, line);
	LexItem tok;
	if (!t1)
	{
		return false;
	}
	tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		return false;
	}
	while (tok == PLUS || tok == MINUS)
	{
		t1 = Term(in, line);
		if (!t1)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			//cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}

bool Term(istream &in, int &line)
{
	//cout << "In term" << endl;
	bool t1 = SFactor(in, line);
	LexItem tok;
	if (!t1){
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while (tok == MULT || tok == DIV){
		t1 = SFactor(in, line);
		if (!t1){
			ParseError(line, "Missing operand after operator in Term");
			return false;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}

bool SFactor(istream &in, int &line) // SFactor ::= [(+ | -)] Factor
{
	LexItem tok = Parser::GetNextToken(in, line);
	int x = 0;
	// cout << tok.GetLexeme() << endl;
	
	if (tok == PLUS || tok == MINUS){
		if(tok == PLUS){
			x= 1;
		}
		else{
			x=-1;
		}
	}
	else{
		Parser::PushBackToken(tok);
	}

	bool t1= Factor (in, line, x);
	if (!t1){
		//ParseError(line, "Missing operand after operator");
		return false;
	}
	return true;
}

bool LogicExpr(istream &in, int &line) // LogicExpr ::= Expr (= | > | <) Expr
{
	bool t1=  Expr(in, line);
	if (!t1){
		ParseError(line, "Error Expr in LogicExpr 1");
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok != EQUAL && tok != GTHAN && tok != LTHAN){
		ParseError(line, "Error no bool in LogicExpr");
		return false;
	}

	bool t2=  Expr(in, line);
	if (!t2){
		ParseError(line, "Error Expr in LogicExpr 2");
		return false;
	} 
	return true; 
}

bool Var(istream &in, int &line) // Var ::= IDENT
{	
	LexItem tok = Parser::GetNextToken(in, line); 
	if(defVar.find(tok.GetLexeme()) != defVar.end()){ // its present in map
		return true;
	}
	else{
		ParseError(line, "Using Undefined Variable"); 
		return false;
	}
	return true; 
}

bool Factor(istream &in, int &line, int sign) // Factor ::= IDENT | ICONST | RCONST | SCONST | (Expr)
{
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok == IDENT){
		Parser::PushBackToken(tok);
		bool t1= Var(in, line);
		if(!t1){
			return false;
		}
		return true;
	}
	else if (tok == ICONST){
		return true;
	}
	else if (tok == RCONST){
		return true;	
	}
	else if (tok == SCONST){
		return true;
	}
	else if (tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern in Factor");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	else{
		if (tok != LPAREN){
				ParseError(line, "Missing Left Parenthesis in Factor");
				return false;
			}

		bool ex = Expr(in, line);

		if (!ex){
			ParseError(line, "Missing Expr in Factor");
			return false;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok != RPAREN){
			ParseError(line, "Missing Right Parenthesis in Factor");
			return false;
		}
	}
	return true; 
}
