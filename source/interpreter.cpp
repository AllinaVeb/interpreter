#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>

using namespace std;

enum OPERATOR{
	LBRACKET, RBRACKET,
//	ASSING,
	PLUS, MINUS,
	MULTIPLY
};

enum LEXEM_TYPE{
	NUMBER, OPER,
	VARIABLE
};

char OPERATOR_STRING[] = {
	'(', ')',
//	'=',
	'+', '-',
	'*'
};

int PRIORITY [] = {
	-1, -1,
//	0,
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
	Number *getValue(Number *left, Number *right);
};

Number * Oper::getValue(Number *leftnum, Number *rightnum){
	int left = leftnum->getValue();
        int right = rightnum->getValue();
	switch(getType()){
		case PLUS:
			{
				left = left + right;
				break;
			}
		case MINUS:
			{
				left = left - right;
				break;
			}
		case MULTIPLY:
			{
				left = left * right;
				break;
			}
	}
	return new Number(left);
}

class Variable: public Lexem {
	string name;
public:
	Variable(const string &newName){
		name = newName;
		lexem_type = VARIABLE;
	}
	string getName(){
		return name;
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
	int flagFirstOper = 0;
	for(int i = 0; i < infix.size(); i++){
		if(infix[i]->getLexem() == NUMBER){
			postfix.push_back(infix[i]);
			continue;
		}
		if(infix[i]->getLexem() == VARIABLE){
			postfix.push_back(infix[i]);
			continue;
		}
		if(infix[i]->getLexem() == OPER){
			if(flagFirstOper == 0){
				stack.push((Oper *)infix[i]);
				flagFirstOper = 1;
				continue;
			}
			Oper *x = stack.top(); 
			if(x->getPriority() < ((Oper *)infix[i])->getPriority()){
				stack.push((Oper *)infix[i]);
				continue;
			}
			if(x->getPriority() == ((Oper *)infix[i])->getPriority()){
				postfix.push_back(x);
				stack.pop();
				stack.push((Oper *)infix[i]);
				continue;
			}
			if(x->getPriority() > ((Oper *)infix[i])->getPriority()){ 
				if(((Oper *)infix[i])->getType() == LBRACKET){
					stack.push((Oper *)infix[i]);
					continue;
				}
				if(((Oper *)infix[i])->getType() == RBRACKET){
					while(x->getType() != LBRACKET){
						postfix.push_back(x);	
						stack.pop();
						x = stack.top();
					}
					stack.pop(); //pop (
					continue;
				}
				else{
					postfix.push_back(x);
					stack.pop();
					stack.push((Oper *)infix[i]);
					continue;
				}
			}
		}
	}
	cout << "size of stack " << stack.size() << endl;
	int sizestack = stack.size();
	for(int i = 0; i < sizestack; i++){
		Oper *x = stack.top();
		cout << "stack[" << i << "] is oper "
                             << OPERATOR_STRING[(int)(x->getType())] << endl;
		if(!(x->getType() == LBRACKET)){
			postfix.push_back(x);
		}
		stack.pop();
	}
	cout << "size of postfix " << postfix.size() << endl;
	return postfix;
}

int evaluatePoliz(vector<Lexem *> poliz){
	stack<Number *> ans;
	for(int i = 0; i < poliz.size(); i++){
		if(poliz[i]->getLexem() == NUMBER){
			ans.push((Number *)poliz[i]);
			continue;
		}
		if(poliz[i]->getLexem() == OPER){
			int size = ans.size();
			cout << "size of ans = " << size << endl;
			Number *x = ans.top();
			cout << "left num is " << x->getValue() << endl;
			ans.pop();
			Number *y = ans.top();
			cout << "right num is " << y->getValue() << endl;
			ans.pop();
			Number *ptrN =((Oper *)poliz[i])->getValue(y, x);
                        ans.push(ptrN);
			continue;
		}
	}
	return (ans.top())->getValue();
}

void printVec(vector<Lexem *> postfix){
	for(int i = 0; i < postfix.size(); i++){
		int k = (int)postfix[i]->getLexem();
		if(k == 0){
			cout << "postfix[" << i << "] is num " 
			     << ((Number *)postfix[i])->getValue() << endl;
			continue;
		}
		if(k == 1){
			cout << "postfix[" << i << "] is oper " 
			     << OPERATOR_STRING[(int)(((Oper *)postfix[i])->getType())] << endl;
			continue;
		}
		if(k == 2){
			cout << "postfix[" << i << "] is var " 
			     << (string)(((Variable *)postfix[i])->getName()) << endl;
			continue;
		}
	}
}

int main(){
	string codeline;
	vector<Lexem *> infix;
	vector<Lexem *> postfix;
	int value;
	while(getline(cin, codeline)){
		infix = parseLexem(codeline);
		cout << "size of infix " << infix.size() << endl;
		postfix = buildPoliz(infix);
		printVec(postfix);
		value = evaluatePoliz(postfix);
		cout << "value = " << value << endl ;
	}
	return 0;
}
