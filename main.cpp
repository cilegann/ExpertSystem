#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>

using namespace std;

//函數宣告
int countlines(char*);
string readline(char*,int);
struct fact;
struct statement;
class tree;
vector<tree> treeVec;
vector<fact> factVec;
vector<statement> statementVec;

//事實結構
struct fact{
    string conclusion;
    string state;
    string prompt;
    fact();
    fact(string conclusion){
        this->conclusion=conclusion;
        this->state="0";
        this->prompt="X";
    }
    
    bool operator == (const fact& rhs) const{
        return conclusion == rhs.conclusion;
    }
    string getConclusioln(){
        return conclusion;
    }
};

//小顆狀態結構
struct statement{
    string condition;
    string shouldBeState;
    bool ifStateEqual;
    statement(){}
    statement(string cond,string shouldbe){
        this->condition=cond;
        this->shouldBeState=shouldbe;
        this->ifStateEqual=0;
    }
    bool operator==(const statement& rhs) const{
        return (condition == rhs.condition)&&(shouldBeState == rhs.shouldBeState);
    }
};

void factAdder(string conclu){
    int exist=-1;
    for(int i=0;i<factVec.size();i++){
        if(factVec[i].getConclusioln()==conclu){
            exist=i;
        }
    }
    
    if(exist==-1){
        factVec.push_back(fact(conclu));
    }
}

//STATEMENT CHECKER AND ADDER
statement* statementAdder(string conclusion, string shouldBeState){
    int exist=-1;
    statement* toReturn;
    for(int i=0;i<statementVec.size();i++){
        if(statementVec[i].condition==conclusion && statementVec[i].shouldBeState==shouldBeState){
            exist=i;
            break;
        }
    }
    if(exist!=-1){
        toReturn=&(statementVec[exist]);
    }else{
        statement stateTemp =statement(conclusion,shouldBeState);
        statementVec.push_back(stateTemp);
        factAdder(conclusion);
        toReturn = &stateTemp;
    }
    return toReturn;
}


//邏輯結構，包含名稱(rule name)、sug(suggestion)、result statement*、OR vector<AND vector<狀態結構*in statement vector>>
class tree{
public:
    string name;
    string suggestion;
    statement* resultStatement;
    vector<vector<statement* > >  OR;
    
    tree(){}
    tree(string name,string conclusion,string shouldBeState){
        this->name=name;
        resultStatement = statementAdder(conclusion, shouldBeState);
    }
    tree(string name,string conclusion,string shouldBeState,string suggestion){
        this->name=name;
        resultStatement = statementAdder(conclusion, shouldBeState);
        this->suggestion=suggestion;
    }
    
    bool operator==(const tree& rhs) const{
        return name == rhs.name;
    }
    
    void orPusher(vector<statement*> andvec){
        OR.push_back(andvec);
    }
    
    void printSug(){
        cout<<"BOT>"<<suggestion;
    }
    
};


//TREE ADDER
//[REVIEWED]
void treeAdder(string ruleName,string conclusion,string conclusionShouldBeState,string suggestion,string ifUncut){
    tree* t;
    int exist=-1;
    for(int i=0;i<treeVec.size();i++){
        if(treeVec[i].name==ruleName){
            exist=i;
            break;
        }
    }
    if(exist!=-1){
        t=(&treeVec[exist]);
    }else{
        t =new tree(ruleName,conclusion,conclusionShouldBeState,suggestion);
        treeVec.push_back(*t);
    }
    
    vector<statement*> andVec;
    int numOfCondition=1;
    for(int i=0;i<ifUncut.length();i++){
        if(ifUncut[i]=='&'){
            numOfCondition++;
        }
    }
    int conditionStartIndex=3;
    int conditionEndIndex;
    int equalIndex;
    int shouBeIndex;
    int andIndex=1;
    string conditionUncut; //IF is_mammal = T & is_carnivore = T
    string condition;  //is_mammal
    string shouldBe;  //T
    equalIndex = (int)ifUncut.find('=');
    conditionEndIndex = equalIndex-1;
    condition = ifUncut.substr(conditionStartIndex,conditionEndIndex - conditionStartIndex);
    shouBeIndex = equalIndex+2;
    shouldBe = ifUncut.substr(shouBeIndex,1);
    statement* s = statementAdder(condition, shouldBe);
    andVec.push_back(s);
    if(numOfCondition > 1){
        for(int i = 1;i < numOfCondition;i++){
            string conditionUncut; //IF is_mammal = T & is_carnivore = T
            string condition;  //is_mammal
            string shouldBe;  //T
            andIndex = (int)ifUncut.find('&',andIndex+1);
            equalIndex = (int)ifUncut.find('=',equalIndex+1);
            conditionEndIndex = equalIndex-1;
            conditionStartIndex=andIndex+2;
            condition = ifUncut.substr(conditionStartIndex,conditionEndIndex - conditionStartIndex);
            shouBeIndex = equalIndex+2;
            shouldBe = ifUncut.substr(shouBeIndex,1);
            //statement* s2 = statementAdder(condition, shouldBe);
            andVec.push_back(statementAdder(condition, shouldBe));
        }
    }
    t->orPusher(andVec);
}






void openrule(string filepath){
    char* filepathChar=&filepath[0u];
    int totalLine=countlines(filepathChar);
    for(int i=0;i<totalLine;i++){
        string str=readline(filepathChar, i);
        if(str[0]=='R'){
            int rulenameLine=i;
            int spaceIndex=(int)str.find('_');
            string ruleName=str.substr(5,spaceIndex-5);
            string ifStringUncut=readline(filepathChar,++rulenameLine);
            string concluStringUncut=readline(filepathChar,++rulenameLine);
            string suggestion;
            if(readline(filepathChar, ++rulenameLine)[0]=='S'){
                suggestion=readline(filepathChar,++rulenameLine);
            }else{
                suggestion="X";
            }
            int equalIndex=(int)concluStringUncut.find('=');
            string conclusion=concluStringUncut.substr(5,equalIndex-6);
            string shouldBestateString=concluStringUncut.substr(equalIndex+2,1);
            bool shouldBestate;
            if(shouldBestateString=="T"){
                shouldBestate=1;
            }else{
                shouldBestate=0;
            }
            
            factAdder(conclusion);
            treeAdder(ruleName, conclusion, shouldBestateString,suggestion,ifStringUncut);
            
        }
    }
    
}


int main(){
    openrule("/Users/sean/Desktop/dp/A_rules.txt");
    return 0;
}

//countline for file stream
int countlines(char* file){
    fstream filetoopen;
    filetoopen.open(file,ios::in);
    int lines=0;
    string tmp;
    if(filetoopen.fail()){
        return 0;
    }else{
        while(getline(filetoopen,tmp)){
            lines++;
        }
        filetoopen.close();
        return lines;
    }
}

//readline for file stream
string readline(char* file,int target_line){
    fstream filetoopen;
    filetoopen.open(file,ios::in);
    string line;
    int i=0;
    while(getline(filetoopen,line)&&i<target_line){
        i++;
    }
    return line;
}



//
//                       _oo0oo_
//                      o8888888o
//                      88" . "88
//                      (| -_- |)
//                      0\  =  /0
//                    ___/`---'\___
//                  .' \\|     |// '.
//                 / \\|||  :  |||// \
//                / _||||| -:- |||||- \
//               |   | \\\  -  /// |   |
//               | \_|  ''\---/''  |_/ |
//               \  .-\__  '-'  ___/-. /
//             ___'. .'  /--.--\  `. .'___
//          ."" '<  `.___\_<|>_/___.' >' "".
//         | | :  `- \`.;`\ _ /`;.`/ - ` : | |
//         \  \ `_.   \_ __\ /__ _/   .-` /  /
//     =====`-.____`.___ \_____/___.-`___.-'=====
//                       `=---='
//
//
//     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//               佛祖保佑         永無bug
//
//***************************************************
//
//   █████▒█    ██  ▄████▄   ██ ▄█▀       ██████╗ ██╗   ██╗ ██████╗
// ▓██   ▒ ██  ▓██▒▒██▀ ▀█   ██▄█▒        ██╔══██╗██║   ██║██╔════╝
// ▒████ ░▓██  ▒██░▒▓█    ▄ ▓███▄░        ██████╔╝██║   ██║██║  ███╗
// ░▓█▒  ░▓▓█  ░██░▒▓▓▄ ▄██▒▓██ █▄        ██╔══██╗██║   ██║██║   ██║
// ░▒█░   ▒▒█████▓ ▒ ▓███▀ ░▒██▒ █▄       ██████╔╝╚██████╔╝╚██████╔╝
//  ▒ ░   ░▒▓▒ ▒ ▒ ░ ░▒ ▒  ░▒ ▒▒ ▓▒       ╚═════╝  ╚═════╝  ╚═════╝
//  ░     ░░▒░ ░ ░   ░  ▒   ░ ░▒ ▒░
//  ░ ░    ░░░ ░ ░ ░        ░ ░░ ░
//           ░     ░ ░      ░  ░
//                 ░
