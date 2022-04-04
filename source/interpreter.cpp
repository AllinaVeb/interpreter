#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>

using namespace std;

map<string, int> table;
map<string, int> LabelTable;

enum OPERATOR{
	IF, THEN,
	ELSE, ENDIF,
	WHILE, ENDWHILE,
	GOTO, ASSIGN, COLON,
	LBRACKET, RBRACKET,
	OR,
	AND,
	BITOR,
	XOR,
	BITAND,
	EQ, NEQ,
	SHL,SHR,
	LEQ, LT,
	GEQ, GT,
	PLUS, MINUS,
	MULT, DIV, MOD
};

enum LEXEM_TYPE{
	NUMBER, OPER,
	VARIABLE
};

string OPERTEXT[] = {
	"if", "then",
	"else", "endif",
	"while", "endwhile",
	"goto", "=", ":",
	"(", ")",
	"or",
	"and",
	"|",
	"^",
	"&",
	"==", "!=",
	"<<", ">>",
	"<=", "<",
	">=", ">",
	"+", "-",
	"*", "/", "%"
};

int PRIORITY [] = {
	-2, -2,
	-2, -2,
	-2, -2,
	-1, -1, -1,
	0, 0,
	1,
	2,
	3,
	4,
	5,
	6, 6,
	8, 8,
	7, 7,
	7, 7, 
	9, 9,
	10, 10, 10
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
        int getValue(){
		return table[name];
	}
        void setValue(int newValue){
        //      value = newValue;
        //      work in map??
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
	Lexem *getValue(Lexem *, Lexem *);
};

Lexem * Oper::getValue(Lexem *leftarg, Lexem *rightarg){
	int left, right;
	if(getType() == ASSIGN){
                right = ((Number *)rightarg)->getValue();
                string name = ((Variable *)leftarg)->getName();
                cout << "name is " << name << endl;
                table[name] = right;
                cout << "map is " << table[name] << endl;
                return new Number(right);
        }
        if(leftarg->getLexem() == NUMBER){
                left = ((Number *)leftarg)->getValue();
        }
        else{
                left =table[((Variable *)leftarg)->getName()];
        }
        if(rightarg->getLexem() == NUMBER){
                right = ((Number *)rightarg)->getValue();
        }
        else{
                right = table[((Variable *)rightarg)->getName()];
        }
	switch(getType()){
		case PLUS:
			{
				right = left + right;
				break;
			}
		case MINUS:
			{
				right = left - right;
				break;
			}
		case MULT:
			{
				right = left * right;
				break;
			}
		case OR:
			{
				right = left or right;
				break;
			}
		case AND:
			{
				right = left and right;
				break;
			}
		case BITOR:
			{
				right = left bitor right;
				break;
			}
		case XOR:
			{
				right = left xor right;
				break;
			}
		case BITAND:
			{
				right = left bitand right;
				break;
			}
		case EQ:
			{
				right = (left == right);
				break;
			}
		case NEQ:
			{
				right = (left != right);
				break;
			}
		case LEQ:
			{
				right = left <= right;
				break;
			}
		case LT:
			{
				right = left < right;
				break;
			}
		case GEQ:
			{
				right = left >= right;
                                break;
                        }
                case GT:
                        {
                                right = left > right;
                                break;
                        }
                case SHL:
                        {
                                right = left << right;
                                break;
                        }

                case SHR:
                        {
                                right = left >> right;
                                break;
                        }
                case DIV:
                        {
                                right = left / right;
                                break;
                        }

                case MOD:
                        {
                                right = left % right;
                                break;
                        }
	}
	return new Number(right);
}

Oper *checkOper(string codeline, int *i){
	int size = sizeof(OPERTEXT)/sizeof(OPERTEXT[0]);
	int position = -1;
	string subline;
	for(int j = 0; j < size; j++){
		string subcodeline = codeline.substr(*i, OPERTEXT[j].size());
		if(OPERTEXT[j] == subcodeline){
			position = j;
			subline = subcodeline;
		}
	}
	if(position >= 0){
			cout << "oper is " << subline << endl;
			if(subline.size() == 2){
                                (*i)++;
                        }
                        if(subline.size() == 3){
                                (*i)++;
                                (*i)++;
                        }
			return new Oper((OPERATOR)position);
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
	string name;
	if((codeline[*i] >= 'a' && codeline[*i] <= 'z') || 
	   (codeline[*i] >= 'A' && codeline[*i] <= 'Z')){
		while((codeline[*i] >= 'a' && codeline[*i] <= 'z') || isdigit(codeline[*i])
			|| (codeline[*i] >= 'A' && codeline[*i] <= 'Z')){
			string newLetter = {codeline[*i]};
                        name.append(newLetter);
                        (*i)++;
		}
		cout << "variable is " << name << endl;
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
			i--;
			continue;
		}
		Oper *ptrO = checkOper(codeline, &i);
                if(ptrO){
                        infix.push_back(ptrO);
			continue;
                }
		Variable *ptrV = checkLetter(codeline, &i);
                if(ptrV){
                        infix.push_back(ptrV);
			i--;
                        continue;
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
				if(((Oper *)infix[i])->getType() == LBRACKET){
					stack.push((Oper *)infix[i]);
                                        continue;
				}
				if(!(((Oper *)infix[i])->getType() == RBRACKET)){
					postfix.push_back(x);
					stack.pop();
					stack.push((Oper *)infix[i]);
					continue;
				}else{
					stack.pop();
					if(stack.empty()){
                                		flagFirstOper = 0;
                                	}
					continue;
				}	
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
					if(stack.empty()){
						flagFirstOper = 0;
					}
					continue;
				}
				else{	
					while(!(stack.empty())){
						x = stack.top();
        	        	                postfix.push_back(x);
	        	                        stack.pop();
					}
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
                             << OPERTEXT[(int)(x->getType())] << endl;
		if(!(x->getType() == LBRACKET)){
			postfix.push_back(x);
		}
		stack.pop();
	}
	cout << "size of postfix " << postfix.size() << endl;
	return postfix;
}

int evaluatePoliz(vector<Lexem *> poliz){
	stack<Lexem *> ans;
	ans.push(new Number(0)); //for -num 
	for(int i = 0; i < poliz.size(); i++){
		if(poliz[i]->getLexem() != OPER){
			ans.push(poliz[i]);
			continue;
		}
		if(poliz[i]->getLexem() == OPER){
			cout << "size of ans = " << ans.size() << endl;
			Lexem *x = ans.top();
			ans.pop();
			Lexem *y = ans.top();
			ans.pop();
		//	cout << "left arg is " << y->getValue() << endl;
		//	cout << "right arg is " << x->getValue() << endl;
			Lexem *arg =((Oper *)poliz[i])->getValue(y, x);
			if(ans.empty()){
				ans.push(new Number(0));
			}
                        ans.push(arg);
			continue;
		}
	}
	return ((Number *)ans.top())->getValue();
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
			     << OPERTEXT[(int)(((Oper *)postfix[i])->getType())] << endl;
			continue;
		}
		if(k == 2){
			cout << "postfix[" << i << "] is var " 
			     << (string)(((Variable *)postfix[i])->getName()) << endl;
			continue;
		}
	}
}

void initLabels(vector<Lexem *> &infix, int &row){
        cout << "row : " << row << endl;
        auto elem = infix.begin();
        for(int i = 1; i < infix.size(); i++){
                if((int)infix[i - 1]->getLexem() == VARIABLE &&
                   (int)infix[i]->getLexem() == OPER){
                        if(((Oper *)infix[i])->getType() == COLON){
                                LabelTable[((Variable *)infix[i - 1])->getName()] = row;
                                cout << "we find : " << endl;
                                elem += i - 1;
                                infix.erase(elem);
                                infix.erase(elem);
                                i++;
                        }
                }
        }
        return;
}

int main(){
	string codeline;
	vector<vector<Lexem *>> infixLines, postfixLines;
	while(getline(cin, codeline)){
		infixLines.push_back(parseLexem(codeline));
        }
	int row;
	        for(row = 0; row < infixLines.size(); row++){
                initLabels(infixLines[row], row);
                printVec(infixLines[row]);
        }
        for(row = 0; row < infixLines.size(); row++){
                postfixLines.push_back(buildPoliz(infixLines[row]));
                printVec(postfixLines[row]);
        }
	row = 0;
	//evaluate
	return 0;
}
