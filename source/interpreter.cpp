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
	-2, 0, -2,
	-1, -1,
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
	Lexem(){
		cout << "NEW LEXEM" << endl;
	}
	~Lexem(){
		cout << "DELETE LEXEM" << endl;
	}
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
        	table[name] = newValue;
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

class Goto: public Oper {
	int row;
public:
	Goto(OPERATOR opertype): Oper(opertype){
		row = -1;
	}
	void setRow(int newrow){
		row = newrow;
	}
	int getRow(){
		return row;
	}
};

Lexem * Oper::getValue(Lexem *leftarg, Lexem *rightarg){
	int left, right;
	if(getType() == ASSIGN){
                right = ((Number *)rightarg)->getValue();
		((Variable *)leftarg)->setValue(right);
               string name = ((Variable *)leftarg)->getName();
		return new Number(right);
        }
        if(leftarg->getLexem() == NUMBER){
                left = ((Number *)leftarg)->getValue();
        }
        else{
                left = ((Variable *)leftarg)->getValue();
        }
        if(rightarg->getLexem() == NUMBER){
                right = ((Number *)rightarg)->getValue();
        }
        else{
                right = ((Variable *)rightarg)->getValue();
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
	for(int j = 0; j < size; j++){
		string subcodeline = codeline.substr(*i, OPERTEXT[j].size());
		if(OPERTEXT[j] == subcodeline){
			cout  << "[" <<  subcodeline << "] ";
			for(int k = 1; k < OPERTEXT[j].size(); k++){ 
                                (*i)++;
                        }
			if(OPERTEXT[j] == "GOTO" || OPERTEXT[j] == "IF" || OPERTEXT[j] == "ELSE" ||
			   OPERTEXT[j] == "WHILE" || OPERTEXT[j] == "ENDWHILE"){
				return new Goto((OPERATOR)j);
			}
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
		cout << "[" << number << "] ";
		(*i)--;
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
		cout << "[" << name << "] ";
		(*i)--;
		return new Variable(name);
	}
	return nullptr;
}

vector<Lexem *>  parseLexem(string codeline){
	vector<Lexem *> infix;
	Lexem* ptr;
	for(int i = 0; i < codeline.size(); i++){
		if(ptr = checkNumber(codeline, &i)){
			infix.push_back(ptr);
			continue;
		}
                if(ptr = checkOper(codeline, &i)){
                        infix.push_back(ptr);
			continue;
                }
                if(ptr = checkLetter(codeline, &i)){
                        infix.push_back(ptr);
                        continue;
                }
	}
	cout << endl;
	return infix;
}

vector<Lexem *> buildPoliz(vector<Lexem *> infix){
	vector<Lexem *> postfix;
	stack<Oper *> stack;
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
			if(stack.empty()){
				stack.push((Oper *)infix[i]);
                                continue;
                        }
			Oper *x = stack.top();
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
	int sizestack = stack.size();
	for(int i = 0; i < sizestack; i++){
		Oper *x = stack.top();
		if(!(x->getType() == LBRACKET)){
			postfix.push_back(x);
		}
		stack.pop();
	}
	cout << '\n' <<  endl;
	return postfix;
}

int evaluatePoliz(vector<Lexem *> poliz, int &row){
	stack<Lexem *> ans;
	Number *num = new Number(0);
	vector<Lexem *> ArrClear;
	ArrClear.push_back(num);
	cout << "arrclear size 1 ? " << ArrClear.size() << endl;
	ans.push(num); //for -num 
	for(int i = 0; i < poliz.size(); i++){
		if(poliz[i]->getLexem() != OPER){
			ans.push(poliz[i]);
			continue;
		}
		if(poliz[i]->getLexem() == OPER){
			if(((Oper *)poliz[i])->getType() == GOTO){ 
				cout << "we find goto" << endl;
				return LabelTable[((Variable *)poliz[i - 1])->getName()];
			}
			Lexem *x = ans.top();
			ans.pop();
			Lexem *y = ans.top();
			ans.pop();
			Lexem *arg =((Oper *)poliz[i])->getValue(y, x);
			if(ans.empty()){
				num = new Number(0);
				ans.push(num);
				ArrClear.push_back(num);
				cout << "arrclear size 1?  " << ArrClear.size() << endl;
			}
                        ans.push(arg);
			ArrClear.push_back(arg);
			cout << " arrclear size " << ArrClear.size() << endl;
			continue;
		}
	}
	cout << "answer is " << ((Number *)ans.top())->getValue() << endl;
	cout << "arrclear size before " << ArrClear.size() << endl;	
	int size = ArrClear.size();
	for(int i = 0; i < size; i++){
		cout << " i = " << i << endl;
		delete ArrClear[i];
	}
	cout << "arrclear size " << ArrClear.size() << endl;
	cout << "we here ?" << endl;
	return row + 1;
}

void printVec(vector<Lexem *> postfix){
	for(int i = 0; i < postfix.size(); i++){
		int k = (int)postfix[i]->getLexem();
		if(k == 0){
			cout << "[" << ((Number *)postfix[i])->getValue() << "] ";
			continue;
		}
		if(k == 1){
			cout << "[" << OPERTEXT[(int)(((Oper *)postfix[i])->getType())] << "] ";
			continue;
		}
		if(k == 2){
			cout << "[" << (string)(((Variable *)postfix[i])->getName()) << "] ";
			continue;
		}
	}
	cout << endl;
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

void initJumps(vector<vector<Lexem *>> infixLines){
	cout << "we are in init jumps " << endl;
	stack<Goto *> stackIfElse;
	Goto *ifif = new Goto((OPERATOR)0);
	cout << "test get row " << ifif->getRow() << endl;
	delete ifif;
	for(int row = 0; row < infixLines.size(); row++){
		for(int i = 0; i < infixLines[row].size(); i++){
			if(infixLines[row][i]->getLexem() == OPER){
				if(((Oper *)infixLines[row][i])->getType() == IF)
					{
				//		cout << "if row " << ((Goto *)ifoper)->getRow() << endl;
						cout << " we have if on row " << row << i << endl;
						/*stackIfElse.push((Goto *)infixLines[row][i]);
						cout << "if on " << row << " row" << endl;
						(stackIfElse.top())->setRow(row);
						cout << ((Goto *)stackIfElse.top())->getRow() << " stacktop row" << endl;
						
						*/}
				if(((Oper *)infixLines[row][i])->getType() == ELSE){
						cout << "we have else on row " << row << i <<  endl;
						/*
						Goto *x = stackIfElse.top();
						x->setRow(row + 1);
						cout << "if[row] = " << row + 1 << endl;
						stackIfElse.pop();
						stackIfElse.push((Goto *)infixLines[row][i]);
					*/
						}
				/*	case ENDIF:{
						stackIfElse.top()->setRow(row + 1);
						cout << "else row? = " << row + 1 << endl;
						stackIfElse.pop();
					}*/
			
			}
		}
	}
}

void infixClear(vector<Lexem *> &infix){
	cout << " we are in infix clear " << endl;
	cout << "infix size is " << infix.size() << endl;
	int size = infix.size();
	for(int i = 0; i < size; i++){
		delete infix[i];		
		// AddressSanitizer: new-delete-type-mismatch
		// deleted one and stoped		
	}
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
	initJumps(infixLines);

        for(row = 0; row < infixLines.size(); row++){
                postfixLines.push_back(buildPoliz(infixLines[row]));
                printVec(postfixLines[row]);
        }
	row = 0;
	while(row >= 0 && row < postfixLines.size()){
		row = evaluatePoliz(postfixLines[row], row);
		cout << "row is " << row << endl;
	}
	for(int i = 0; i < infixLines.size(); i++){
		infixClear(infixLines[i]);
	}
	cout << "we in end main ?" << endl;
	infixLines.clear();
	return 0;
}
