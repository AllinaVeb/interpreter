#include <string>
#include <vector>
#include <map>

using namespace std;

extern map<string, int> VarTable;
extern map<string, int> LabelTable;
extern map<string, vector<int>> ArrayTable;

enum OPERATOR{
        IF, THEN,
        ELSE, ENDIF,
        WHILE, ENDWHILE,
        GOTO, ASSIGN, COLON,
        ARRAY,
        LBRACKET, RBRACKET,
        LQBRACKET, RQBRACKET,
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

class Lexem {
public:
        LEXEM_TYPE lexemType;
        Lexem();
        virtual ~Lexem();
        LEXEM_TYPE getLexemType();
};

class Item: public Lexem {
public:
        virtual int getValue() = 0;
};

class Literal: public Item{
public:
        virtual string getName() = 0;
        virtual void setValue(int) = 0;
};

class ArrayElem: public Literal {
        string name;
        int index;
public:
        ArrayElem(string newName, int newIndex);
        string getName();
        int getValue();
        void setValue(int newValue);
};

class Number: public Item {
        int value;
public:
        Number(int newValue);
        int getValue();
};

class Variable: public Literal {
        string name;
public:
        Variable(const string &newName);
        string getName();
        int getValue();
        void setValue(int newValue);
};

class Oper: public Lexem {
        OPERATOR opertype;
public:
        Oper(OPERATOR opertype);
        OPERATOR getType();
        int getPriority();
        Lexem *getValue(Lexem *, Lexem *);
};

class Goto: public Oper {
        int row;
public:
        Goto(OPERATOR opertype);
        void setRow(int newrow);
        int getRow();
};

vector<Lexem *> parseLexem(string);

vector<Lexem *> buildPoliz(vector<Lexem *>);

void cleaner(vector<Lexem *> &);

int evaluatePoliz(vector<Lexem *>, int &);

void printVec(vector<Lexem *>);

void initLabels(vector<Lexem *> &, int &);

void initJumps(vector<vector<Lexem *>>);
