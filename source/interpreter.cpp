#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>

using namespace std;

map<string, int> VarTable;
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
	LEXEM_TYPE lexemType;
	Lexem(){
		cout << "NEW LEXEM" << endl;
	}
	~Lexem(){
		cout << "DELETE LEXEM" << endl;
	}
	LEXEM_TYPE getLexemType(){
		return lexemType;
	}		
};

class Item: public Lexem {
public:
	virtual int getValue() = 0;
};

class Number: public Item {
	int value;
public:
	Number(int newValue){
		value = newValue;
		lexemType = NUMBER;
	}
	int getValue(){
		return value;
	}
};

class Variable: public Item {
        string name;
public:
        Variable(const string &newName){
                name = newName;
                lexemType = VARIABLE;
        }
        string getName(){
                return name;
        }
        int getValue(){
		return VarTable[name];
	}
        void setValue(int newValue){
        	VarTable[name] = newValue;
        }
};

class Oper: public Lexem {
	OPERATOR opertype;
public:
	Oper(OPERATOR opertype){
		Oper::opertype = opertype;
		lexemType = OPER;
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
	switch(getType()){
		case ASSIGN:{
                	right = ((Item *)rightarg)->getValue();
			((Variable *)leftarg)->setValue(right);
			return new Number(right);
        		}
	}
        left = ((Item *)leftarg)->getValue();
        right = ((Item *)rightarg)->getValue();
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
			(*i) += OPERTEXT[j].size() - 1;
			if(j == GOTO || j == IF || j == ELSE || j == WHILE || j == ENDWHILE){
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

bool isletter(char codeletter){
	if((codeletter >= 'a' && codeletter <= 'z') || 
		(codeletter >= 'A' && codeletter <= 'Z')){
		return true;
	}
	return false;
}

Variable *checkLetter(string codeline, int *i){
	string name;
	if(isletter(codeline[*i])){
		while(isletter(codeline[*i]) || isdigit(codeline[*i])){
			name +=codeline[*i];
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
		if(infix[i]->getLexemType() == NUMBER){
			postfix.push_back(infix[i]);
			continue;
		}
		if(infix[i]->getLexemType() == VARIABLE){
			postfix.push_back(infix[i]);
			continue;
		}
		if(infix[i]->getLexemType() == OPER){
			if(((Oper *)infix[i])->getType() == THEN){
				continue;
			}
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
	ans.push(num); //for -num 
	for(int i = 0; i < poliz.size(); i++){
		if(poliz[i]->getLexemType() != OPER){
			ans.push(poliz[i]);
			continue;
		}
		if(poliz[i]->getLexemType() == OPER){
			int oper = (int)(((Oper *)poliz[i])->getType());
			if(oper == GOTO){
				return LabelTable[((Variable *)poliz[i - 1])->getName()];
			}
			if(oper == ELSE || oper == ENDWHILE){
				return ((Goto *)poliz[i])->getRow();
			}
			if(oper == IF || oper == WHILE){
				if(!((Number *)ans.top())->getValue()){
                                                return ((Goto *)poliz[i])->getRow();
                                }
			}
			if(oper == ENDIF){
				return row + 1;
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
			}
                        ans.push(arg);
			ArrClear.push_back(arg);
			continue;
		}
	}
	cout << "answer is " << ((Number *)ans.top())->getValue() << endl;
	int size = ArrClear.size();
	auto elem = ArrClear.begin();
	while(!ArrClear.empty()){
		ArrClear.erase(elem);
	}
/*	for(int i = 0; i < size; i++){
		cout << " i = " << i << endl;
		if(ArrClear[i] != nullptr){
			delete ArrClear[i];
			ArrClear[i] = nullptr;
		}
	}
	*/
	return row + 1;
}

void printVec(vector<Lexem *> postfix){
	for(int i = 0; i < postfix.size(); i++){
		int k = (int)postfix[i]->getLexemType();
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
		if(infix[i - 1] != nullptr && infix[i] != nullptr){
                	if((int)infix[i - 1]->getLexemType() == VARIABLE &&
                   	(int)infix[i]->getLexemType() == OPER){
                        	if(((Oper *)infix[i])->getType() == COLON){
					LabelTable[((Variable *)infix[i - 1])->getName()] = row;
        	                        cout << "we find goto on row  " << row << endl;
                	               // delete infix[i - 1];
					//delete infix[i];
					//infix[i - 1] == nullptr;
					//infix[i] == nullptr;
					elem += i - 1;
                       	        	infix.erase(elem);
                                	infix.erase(elem);
                                	i++;
                        	}
			}
                }
        }
}

void initJumps(vector<vector<Lexem *>> infixLines){
	cout << "we are in init jumps " << endl;
	stack<Goto *> stackIfElse;
	stack<Goto *> stackWhile;
	for(int row = 0; row < infixLines.size(); row++){
		for(int i = 0; i < infixLines[row].size(); i++){
			if(infixLines[row][i]->getLexemType() == OPER){
				Oper *operGoto = (Oper *)infixLines[row][i];
				switch(((Oper *)operGoto)->getType()){
					case IF:{
						cout << "case if row " << row << endl;
						stackIfElse.push((Goto *)operGoto);
						cout << "if on " << row << " row" << endl;
						break;
					}
					case ELSE:{
						cout << "case else row " << row << endl; 
						stackIfElse.top()->setRow(row + 1);
						cout << "if[row] = " << row + 1 << endl;
						stackIfElse.pop();
						stackIfElse.push((Goto *)operGoto);
						break;
					}
					case ENDIF:{
						cout << "case endif row " << row << endl;
						stackIfElse.top()->setRow(row + 1);
						cout << "else row? = " << row + 1 << endl;
						stackIfElse.pop();
						break;
					}
					case WHILE:{
						cout << "case while row " << row << endl;
						((Goto *)operGoto)->setRow(row);
						stackWhile.push((Goto *)operGoto);
						break;
					}
					case ENDWHILE:{
						cout << "case endwhile row " << row << endl;
						int whilerow = stackWhile.top()->getRow();
						((Goto *)operGoto)->setRow(whilerow);
						stackWhile.top()->setRow(row + 1);
						stackWhile.pop();
						break;
					}
				}
			}
		}
	}
}

void infixClear(vector<Lexem *> &infix){
	int size = infix.size();
	auto elem = infix.begin();
/*	for(int i = 0; i < size; i++){
		delete infix[i];		
		// AddressSanitizer: new-delete-type-mismatch
		// deleted one and stoped		
	}
	*/
	while(!infix.empty()){
		infix.erase(elem);
	}

}

int main(){
	string codeline;
	vector<vector<Lexem *>> infixLines, postfixLines;
	while(getline(cin, codeline)){
		infixLines.push_back(parseLexem(codeline));
        }
	int row;
	cout << "looking for goto" << endl;
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
	return 0;
}
