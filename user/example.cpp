#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>

#include <interpreter.h>

using namespace std;

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
        }
        for(int i = 0; i < infixLines.size(); i++){
                cleaner(infixLines[i]);
        }
        return 0;
}


