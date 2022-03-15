#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>

using namespace std;

enum OPERATOR{
	LBRACKET, RBRACKET,
	ASSING,
	PLUS, MINUS,
	MULTIPLY
};

enum LEXEM_TYPE{
	NUMBER, OPER,
	VARIABLE
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
	LEXEM_TYPE lexem_type;
	Lexem(){};
	LEXEM_TYPE getLexem(){
		return lexem_type;
	}		
};

class Number: public Lexem {
	int value;
public:
	Number(int newValue){
		value = newValue;
		lexem_type = NUMBER;
	}
	int getValue(){
		return value;
	}
};

class Oper: public Lexem {
	OPERATOR opertype;
public:
	Oper(OPERATOR opertype){
		Oper::opertype = opertype;
		lexem_type = OPER;
	} 
	OPERATOR getType(){
		return opertype;
	}
	int getPriority(){
		return PRIORITY[getType()];
	}
	int getValue(const Number &left, 
		     const Number &right);
};

class Variable: public Lexem {
	string name;
public:
	Variable(const string &newName){
		name = newName;
		lexem_type = VARIABLE;
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
			return new Oper((OPERATOR)j);
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
                return new Number(number);
        }
        return nullptr;
}

Variable *checkLetter(string codeline, int *i){
	if(codeline[*i] >= 'a' && codeline[*i] <= 'z'){
		cout << "variable is " << codeline[*i] << endl;
		string name = {codeline[*i]};
		return new Variable(name);
	}
	return nullptr;
}

vector<Lexem *>  parseLexem(string codeline){
	vector<Lexem *> infix;
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

vector<Lexem *> buildPoliz(vector<Lexem *> infix){
	vector<Lexem *> postfix;
	stack<Oper *> stack;
	int flag = 0;
	for(int i = 0; i < infix.size(); i++){
		if(infix[i]->getLexem() == NUMBER){
			cout << "infix[" << i << "] is num" << endl;
			postfix.push_back(infix[i]);
			continue;
		}
		if(infix[i]->getLexem() == VARIABLE){
			cout << "infix[" << i << "] is var" << endl;
			postfix.push_back(infix[i]);
			continue;
		}
		if(infix[i]->getLexem() == OPER){
			cout << "infix[" << i << "] is oper" << endl;
		}
/*			if(flag == 0){
				stack.push(oper);
				flag = 1;
				continue;
			}
			Oper x = stack.front() 
			if(x.getPriority() < oper.getPriority()){
				stack.push(oper);
				continue;
			}
			if(x.getPriority() == oper.getPriority()){
				postfix.push(x);
				stack.pop();
				stack.push_back(oper);
				continue;
			}
			if(x.getPriority() > oper.getPriority()){ //oper is bracket
				if(oper == LBRACKET){
					stack.push(oper);
					continue;
				}
				if(oper == RBRACKET){
					while(x != LBRACKET){
						postfix.push_back(x);	
						stack.pop();
						x = stack.front();
					}
					continue;
				}
				else{
					postfix.push_back(x);
					stack.pop();
					stack.push_back(oper);
					continue;
				}
			}
		}
	}
	for(int i = 0; i < stack.size(); i++){
		Oper x = stack.front();
		postfix.push_back(x);
		stack.pop();
	}
*/
	}
	cout << "size of postfix " << postfix.size() << endl;
	for(int i = 0; i < postfix.size(); i++){
		cout << postfix[i]->getLexem() << endl;
	}
	return postfix;
}
/*
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
		cout << "size of codeline " << codeline.size() << endl;
		infix = parseLexem(codeline);
		cout << "size of infix " << infix.size() << endl;
		postfix = buildPoliz(infix);
	//	value = evaluatePoliz(postfix);
	//	cout << value << endl ;
	}
	return 0;
}
