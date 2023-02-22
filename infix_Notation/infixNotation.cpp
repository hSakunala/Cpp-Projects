#include <iostream>
#include <string>
#include <stack>

using namespace std;

 int precedence(char ch){
	if(ch == '+' || ch == '-'){
		return 1;
	}
	else if(ch == '*' || ch == '/'){
		return 2;
	}
	else{
		return 0;
	}

}

void infToPostfix(string instr){
	stack<char> stook;
	stook.push('@');
	//cout <<"Stook first: " << stook.top() << endl;
	string out = "";
	int len= instr.length();

	for(int i=0; i< len; i++){
		char lett= char(instr[i]);
		if (isspace(lett)){
			continue;
		}
		else if (isalpha(lett)){
			out+= lett;
		}
		else if(lett == '('){
			stook.push(lett);
		}
		else if (lett == ')'){
			while (stook.top() != '@' && stook.top() != '('){
				char poop = stook.top();
				out += poop;
				stook.pop();
			}
			stook.pop();
		}
		else{
			if (precedence(lett) > precedence(stook.top())){
				stook.push(lett);
			}
			else{
				while (stook.top()!= '@' && precedence(lett) <= precedence(stook.top())){
					out += stook.top();
					stook.pop();
				}
				stook.push(lett);
			}
		}
	}

	while (stook.top()!= '@'){
		out+= stook.top();
		stook.pop();
	}
	for(int z =0; z< out.length(); z++){
		cout << out[z] << " ";
	}
	cout << endl;

}

int main ( ) 
{
	string instr;
	cout<<"Please enter an infix notation expression using single lowercase characters:" << endl;
	getline(cin, instr);
	infToPostfix(instr);
	
	return 0;	
}