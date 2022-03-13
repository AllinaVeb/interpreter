#include <iostream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

enum OPERATOR {
	LBRACKET, RBRACKET,
	ASSING,
	PLUS, MINUS,
	MULTIPLY
};

char OPERATOR_STRING[] = {
	'(', ')',
	'=',
	'+', '-',
	'*'
};

int PRIORITY [] = {
	-1, -1,
	0,
	1, 1,
	2
};

class Lexem {
public:
	Lexem();
};

class Number: public Lexem {
	int value;
public:
	Number(int newValue){
		value = newValue;
	}
	int getValue(){
		return value;
	}
};

class Oper: public Lexem {
	OPERATOR opertype;
public:
	Oper(OPERATOR opertype);
	OPERATOR getType(){
		return opertype;
	}
	int getPriority(){
		return PRIORITY[getType()];
	}
	int getValue(const Number & left, 
		     const Number & right);
};

class Variable: public Lexem {
	string name;
public:
	Variable(const string &newName){
		name = newName;
	}
	int getValue();
	void setValue(int newValue){
	//	value = newValue;
	}
};

Oper *checkOper(string codeline, int *i){
	for(int j = 0; j < sizeof(OPERATOR_STRING); j++){
		if(codeline[*i] == OPERATOR_STRING[j]){
			cout << "oper is " << OPERATOR_STRING[j] << endl;
		//	return new Oper();
		}
	}
	return nullptr;
}

Number *checkNumber(string codeline, int *i){
	int number = 0;
        if(isdigit(codeline[*i])){
		while(isdigit(codeline[*i])){
			number = number * 10 + (codeline[*i] - '0');
                        (*i)++;
                }
		cout << "number is " << number << endl;
                //return new Number();
        }
        return nullptr;
}

Variable *checkLetter(string codeline, int *i){
	if(codeline[*i] >= 'a' && codeline[*i] <= 'z'){
		cout << "variable is " << codeline[*i] << endl;
	//	return new Variable();
	}
	return nullptr;
}

vector<Lexem *>  parseLexem(string codeline){
	vector <Lexem *> infix;
	for(int i = 0; i < codeline.size(); i++){
		Number *ptrN = checkNumber(codeline, &i);
		if(ptrN){
			infix.push_back(ptrN);
		}
		Variable *ptrV = checkLetter(codeline, &i);
                if(ptrV){
                        infix.push_back(ptrV);
                }
		Oper *ptrO = checkOper(codeline, &i);
                if(ptrO){
                        infix.push_back(ptrO);
                }
	}
	return infix;
}
/*
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
*/
int main(){
	string codeline;
	vector<Lexem *> infix;
	vector<Lexem *> postfix;
	int value;
	while(getline(cin, codeline)){
		infix = parseLexem(codeline);
	//	postfix = buildPoliz(infix);
	//	value = evaluatePoliz(postfix);
	//	cout << value << endl ;
	}
	return 0;
}
