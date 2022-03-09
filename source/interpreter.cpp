#include <iostream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

enum OPERATOR {
	LBRACKET, RBRACKET,
	PLUS, MINUS,
	MULTIPLY
};

int PRIORITY [] = {
	-1, -1,
	0, 0,
	1
};

class Lexem {
public:
	Lexem();
};

class Number: public Lexem {
	int value;
public:
	Number(int value){
		Number::value = value;
	}
	int getValue(){
		return value;
	}
};

class Oper: public Lexem {
	OPERATOR opertype;
public:
	Oper(OPERATOR opertype);
	OPERATOR getType();
	int getPriority();
	int getValue(const Number & left, 
		     const Number & right);
};

vector<Lexem *>  parseLexem(string codeline){
	vector <Lexem *> infix;
	for(int i = 0; i < codeline.size(); i++){
		int number = 0;
		if(isdigit(codeline[i])){
			while(isdigit(codeline[i])){
				number = number * 10 + (codeline[i] - '0');
				i++;
			}
			infix.push_back(new Number(number));
		}
		switch(codeline[i]){
			case '+':
			{
				//push +
			}
			case '-':
			{
				//push -
			}
			case '*':
			{
				//push *
			}
			case '(':
			{
				// push (
			}
			case ')':{
					 //push )
			}
		}
	}
	return infix;
}

vector<Lexem *> buildPoliz(vector<Lexem *> infix){
	vector<Lexem *> postfix;
	stack <int> stack;
	for(int i = 0; i < infix.size(); i++){
		//if(infix[i] number){
		//	add in postfix;
		//}
		//if(infix[i] oper){
		//	int x = stack.top?
		//	if(x == oper){
		//		add in postfix;
		//	}
		//	if(x > oper (oper is bracket)){
		//		if LBRACKET stack.push(?);
		//		if RBRACKET{
		//			while(x != LBRACKET){
		//				x add in postfix;
		//				x = stack.top();
		//			}
		//		}
		//	}
		//	if(x > oper){
		//		while(x > oper){
		//			x add in postfix;
		//			x = stack.top();
		//			}
		//		}
		//}
	}
	return postfix;
}

int evaluatePoliz(vector<Lexem *> poliz){
	int value = 0;
	for(int i = 0; i < poliz.size(); i++){
		//?
	}
	return value;
}

int main () {
	string codeline ;
	vector<Lexem *> infix ;
	vector<Lexem *> postfix ;
	int value ;
	while(getline(cin, codeline)){
		infix = parseLexem(codeline);
		postfix = buildPoliz(infix);
		value = evaluatePoliz(postfix);
		cout << value << endl ;
	}
	return 0;
}
