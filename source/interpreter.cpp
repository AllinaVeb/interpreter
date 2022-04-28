#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>

#include <interpreter.h>

using namespace std;

map<string, int> VarTable;
map<string, int> LabelTable;
map<string, vector<int>> ArrayTable;

string OPERTEXT[] = {
        "if", "then",
        "else", "endif",
        "while", "endwhile",
        "goto", ":",
        "array",
        "(", ")",
        "[", "]",
        "or",
        "and",
        "|",
        "^",
        "&",
        "==", "!=",
	"=",
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
        -2, -2,
        -2,
        -1, -1,
        -1, -1,
        1,
        2,
        3,
        4,
        5,
        6, 6,
	0,
        8, 8,
        7, 7,
        7, 7,
        9, 9,
        10, 10, 10
};

Lexem::Lexem(){};

Lexem::~Lexem(){};

LEXEM_TYPE Lexem::getLexemType(){
                return lexemType;
        }

ArrayElem::ArrayElem(string newName, int newIndex){
                name = newName;
                index = newIndex;
        }

string ArrayElem::getName(){
                return name;
        }

int ArrayElem::getValue(){
                return ArrayTable[name][index];
        }

void ArrayElem::setValue(int newValue){
                ArrayTable[name][index] = newValue;
        }

Number::Number(int newValue){
                value = newValue;
                lexemType = NUMBER;
        }

int Number::getValue(){
                return value;
        }

Variable::Variable(const string &newName){
                name = newName;
                lexemType = VARIABLE;
        }

string Variable::getName(){
                return name;
        }

int Variable::getValue(){
                return VarTable[name];
        }

void Variable::setValue(int newValue){
                VarTable[name] = newValue;
        }

Oper::Oper(OPERATOR opertype){
                Oper::opertype = opertype;
                lexemType = OPER;
        }

OPERATOR Oper::getType(){
                return opertype;
        }

int Oper::getPriority(){
                return PRIORITY[getType()];
        }

Goto::Goto(OPERATOR opertype): Oper(opertype){
                row = -1;
        }

void Goto::setRow(int newrow){
                row = newrow;
        }

int Goto::getRow(){
                return row;
        }

Lexem * Oper::getValue(Lexem *leftarg, Lexem *rightarg){
        int left, right;
        switch(getType()){
                case RQBRACKET:{
                        string name = ((Literal *)leftarg)->getName();
                        int index = ((Item *)rightarg)->getValue();
                        return new ArrayElem(name, index);
                }
                case ASSIGN:{
                        right = ((Item *)rightarg)->getValue();
                        ((Literal *)leftarg)->setValue(right);
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
                        Oper *oper = (Oper *)infix[i];
                        int opertype = oper->getType();
                        if(opertype == THEN){
                                continue;
                        }
                        if(stack.empty()){
                                stack.push(oper);
                                continue;
                        }
                        Oper *x = stack.top();
                        int xtype = x->getType();
                        if(opertype == LBRACKET || opertype == LQBRACKET){
                                        stack.push(oper);
                                        continue;
                        }
                        if(opertype == RBRACKET || opertype == RQBRACKET){
                                while(xtype != LBRACKET && xtype != LQBRACKET ){
                                        postfix.push_back(x);
                                        stack.pop();
                                        x = stack.top();
                                        xtype = x->getType();
                                }
                                if(opertype == RQBRACKET){
                                        postfix.push_back(oper);
                                }
                                stack.pop(); //pop ( or [
                                continue;
                        }
                        if(x->getPriority() < oper->getPriority()){
                                stack.push(oper);
                                continue;
                        }
                        if(x->getPriority() == oper->getPriority()){
                                postfix.push_back(x);
                                stack.pop();
                                stack.push(oper);
                                continue;
                        }
                        if(x->getPriority() > oper->getPriority()){
				while((x->getPriority() > oper->getPriority()) && xtype != LBRACKET && xtype != LQBRACKET && !stack.empty()){
                                        postfix.push_back(x);
                                        stack.pop();
					if(!stack.empty()){
						x = stack.top();
                                        	xtype = x->getType();
					}
                                }
                                stack.push(oper);
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

void cleaner(vector<Lexem *> &ArrClear){
        int size = ArrClear.size();
        for(int i = 0; i < size; i++){
                if(ArrClear[i] != nullptr){
                        delete ArrClear[i];
                        ArrClear[i] = nullptr;
                }
        }
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
                                cleaner(ArrClear);
                                return LabelTable[((Variable *)poliz[i - 1])->getName()];
                        }
                        if(oper == ELSE || oper == ENDWHILE){
                                cleaner(ArrClear);
                                return ((Goto *)poliz[i])->getRow();
                        }
                        if(oper == IF || oper == WHILE){
                                if(!((Number *)ans.top())->getValue()){
                                        cleaner(ArrClear);
                                        return ((Goto *)poliz[i])->getRow();
                                }
                        }
                        if(oper == ENDIF){
                                cleaner(ArrClear);
                                return row + 1;
                        }
                        Lexem *x = ans.top();
                        ans.pop();
                        Lexem *y = ans.top();
                        ans.pop();
                        if(oper == ARRAY){
                                string name = ((Literal *)y)->getName();
                                int arrsize = ((Item *)x)->getValue();
                                ArrayTable[name] = vector<int>(arrsize);
                                cleaner(ArrClear);
                                return row + 1;
                        }
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
        cleaner(ArrClear);
        return row + 1;
}

void printVec(vector<Lexem *> line){
        for(int i = 0; i < line.size(); i++){
                int k = (int)line[i]->getLexemType();
                if(k == 0){
                        cout << "[" << ((Number *)line[i])->getValue() << "] ";
                        continue;
                }
                if(k == 1){
                        cout << "[" << OPERTEXT[(int)(((Oper *)line[i])->getType())] << "] ";
                        continue;
                }
                if(k == 2){
                        cout << "[" << (((Variable *)line[i])->getName()) << "] ";
                        continue;
                }
        }
        cout << endl;
}

void initLabels(vector<Lexem *> &infix, int &row){
        auto elem = infix.begin();
        for(int i = 1; i < infix.size(); i++){
                if(infix[i - 1] != nullptr && infix[i] != nullptr){
                        if((int)infix[i - 1]->getLexemType() == VARIABLE &&
                        (int)infix[i]->getLexemType() == OPER){
                                if(((Oper *)infix[i])->getType() == COLON){
                                        LabelTable[((Literal *)infix[i - 1])->getName()] = row;
                                        delete infix[i - 1];
                                        delete infix[i];
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
                                                stackIfElse.push((Goto *)operGoto);
                                                break;
                                        }
                                        case ELSE:{
                                                stackIfElse.top()->setRow(row + 1);
                                                stackIfElse.pop();
                                                stackIfElse.push((Goto *)operGoto);
                                                break;
                                        }
                                        case ENDIF:{
                                                stackIfElse.top()->setRow(row + 1);
                                                stackIfElse.pop();
                                                break;
                                        }
                                        case WHILE:{
                                                ((Goto *)operGoto)->setRow(row);
                                                stackWhile.push((Goto *)operGoto);
                                                break;
                                        }
                                        case ENDWHILE:{
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

