/**
 ESOE Discrete Math 2017 spring - Final Project
 Expert System
 main.cpp
 @author Chiu,Yu-Hsuan B03505031
 @version 1.1 6/1/17
 */
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
string& trim(string& str);
string& ltrim(string& str);
string& rtrim(string& str);
struct fact;
struct statement;
class tree;
vector<tree> treeVec;
vector<fact> factVec;
vector<statement> statementVec;

//HELP- BOT SAY
void botSay(string tosay){
    cout<<"BOT> "<<tosay<<endl;
}

//STRUCT- 事實結構
struct fact{
    string conclusion;
    string state;
    string prompt;
    fact();
    fact(string conclusion){
        this->conclusion=conclusion;
        this->state="?";
        this->prompt="X";
    }
    fact(string conclusion,string prompt){
        this->conclusion=conclusion;
        this->state="?";
        this->prompt=prompt;
    }
    bool operator == (const fact& rhs) const{
        return conclusion == rhs.conclusion;
    }
    string getConclusioln(){
        return conclusion;
    }
    void setPrompt(string in){
        prompt=in;
    }
};

//HELPER- FACT ADDER
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

//STRUCT- 小顆狀態結構
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

//HELPER- STATEMENT CHECKER AND ADDER
statement statementAdder(string conclusion, string shouldBeState){
    int exist=-1;
    for(int i=0;i<statementVec.size();i++){
        if(statementVec[i].condition==conclusion && statementVec[i].shouldBeState==shouldBeState){
            exist=i;
            break;
        }
    }
    if(exist!=-1){
        return statementVec[exist];
    }else{
        statement stateTemp =statement(conclusion,shouldBeState);
        statementVec.push_back(stateTemp);
        factAdder(conclusion);
        return statementVec.back();
    }
}

//STRUCT- 邏輯結構，包含名稱(rule name)、sug(suggestion)、result statement*、OR vector<AND vector<狀態結構*in statement vector>>
class tree{
public:
    string name;
    string suggestion;
    statement resultStatement;
    vector<vector<statement > >  OR;
    
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
    
    void orPusher(vector<statement> andvec){
        OR.push_back(andvec);
    }
    
    void printSug(){
        botSay(suggestion);
    }
    
};

//HELPER- TREE ADDER
void treeAdder(string ruleName,string conclusion,string conclusionShouldBeState,string suggestion,string ifUncut){
    
    int exist=-1;
    for(int i=0;i<treeVec.size();i++){
        if(treeVec[i].name==ruleName){
            exist=i;
            break;
        }
    }
    if(exist==-1){
        exist=(int)treeVec.size();
        tree t(ruleName,conclusion,conclusionShouldBeState,suggestion);
        treeVec.push_back(t);
    }
    tree& t=treeVec[exist];
    vector<statement> andVec;
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
    andVec.push_back(statementAdder(condition, shouldBe));
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
            andVec.push_back(statementAdder(condition, shouldBe));
        }
    }
    t.orPusher(andVec);
}

//HELPER- PROMPT ADDER
void promptAdder(string conclusion,string promptIn){
    for(int i=0;i<factVec.size();i++){
        if(factVec[i].conclusion==conclusion){
            factVec[i].prompt=promptIn;

        }
    }
}

//MAIN- OPENRULE
void openrule(string filepath){
    int ruleCount=0;
    char* filepathChar=&filepath[0u];
    int totalLine=countlines(filepathChar);
    for(int i=0;i<totalLine;i++){
        string str=readline(filepathChar, i);
        //rule finder
        if(str[0]=='R'){
            ruleCount++;
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
            factAdder(conclusion);
            treeAdder(ruleName, conclusion, shouldBestateString,suggestion,ifStringUncut);
        }else if(str[0]=='P'){
            //add prompt
            string promptName=readline(filepathChar,i);
            int nameSpaceIndex=(int)promptName.find(' ');
            promptName=promptName.substr(nameSpaceIndex+1,promptName.find_last_of(' ')-nameSpaceIndex-1);
            promptAdder(promptName,readline(filepathChar,i+1));
        }
    }
    string tosay=filepath;
    tosay.append(": ");
    tosay.append(to_string(ruleCount));
    tosay.append(" rules read\n");
    botSay(tosay);
}

//MAIN- LIST RULE
void listrule(){
    for(int ti=0;ti<treeVec.size();ti++){
        string tosay=treeVec[ti].name.append(":\n");
        for(int oi = 0 ; oi<treeVec[ti].OR.size();oi++){
            if(oi>0)
                tosay.append(" ^ ");
            tosay.append(" [");
            for(int ai=0 ; ai < treeVec[ti].OR[oi].size() ; ai++){
                if(ai>0)
                    tosay.append(" & ");
                tosay.append("(");
                tosay.append(treeVec[ti].OR[oi][ai].condition);
                tosay.append(" = ");
                tosay.append(treeVec[ti].OR[oi][ai].shouldBeState);
                tosay.append(")");
            }
            tosay.append("] ");
        }
        tosay.append("\n -> ");
        tosay.append(treeVec[ti].resultStatement.condition);
        tosay.append(" = ");
        tosay.append(treeVec[ti].resultStatement.shouldBeState);
        tosay.append("\n");
        botSay(tosay);
    }
}

//MAIN- ADDRULE
void addrule(){
    string todo;
    string in;
    while(cin){
        getline(cin,in);
        if(in=="@")
            break;
        todo.append(in);
    }
    int nameStartIndex=-1;
    int nameEndIndex=-1;
    int ifStartIndex=-1;
    int ifEndIndex=-1;
    int thenStartIndex=-1;
    int thenEndIndex=-1;
    int sugStartIndex=-1;
    int sugEndIndex=-1;
    int promStartIndex=-1;
    int promEndIndex=-1;
    int promNameStartIndex=-1;
    int promNameEndIndex=-1;
    for(int i=0;i<todo.length();i++){
        if(todo[i]=='R'&&todo[i+1]=='U'&&todo[i+2]=='L'&&todo[i+3]=='E'){
            nameStartIndex=i;
            nameEndIndex=i+3;
        }else if(todo[i]=='I'&&todo[i+1]=='F'){
            ifStartIndex=i;
            ifEndIndex=i+1;
        }else if(todo[i]=='T'&&todo[i+1]=='H'&&todo[i+2]=='E'&&todo[i+3]=='N'){
            thenStartIndex=i;
            thenEndIndex=i+3;
        }else if(todo[i]=='S'&&todo[i+1]=='U'&&todo[i+2]=='G'&&todo[i+3]=='G'){
            sugStartIndex=i;
            sugEndIndex=i+8;
        }else if(todo[i]=='P'&&todo[i+1]=='R'&&todo[i+2]=='O'&&todo[i+3]=='M'){
            promNameStartIndex=i;
            promNameEndIndex=(int)todo.find('[',i+1)-1;
        }
    }
    if(nameStartIndex!=-1){
        nameStartIndex=nameEndIndex+1;
        nameEndIndex=(int)todo.find('_',nameStartIndex);
        ifEndIndex=thenStartIndex;
        thenStartIndex=thenEndIndex+1;
        if(sugStartIndex==-1){
            if(promNameStartIndex==-1){
                thenEndIndex=(int)todo.length();
            }else{
                thenEndIndex=promNameStartIndex-1;

            }
        }else{
            thenEndIndex=sugStartIndex;
            sugStartIndex=(int)todo.find('[',sugStartIndex);
            sugEndIndex=(int)todo.find(']',sugEndIndex);
        }
        string ruleName=todo.substr(nameStartIndex,nameEndIndex-nameStartIndex);
        string conditionUncut=todo.substr(ifStartIndex,ifEndIndex-ifStartIndex);
        string conclusionUncut=todo.substr(thenStartIndex,thenEndIndex-thenStartIndex);
        string sug;
        if(sugStartIndex!=-1)
            sug=todo.substr(sugStartIndex,sugEndIndex-sugStartIndex+1);
        else
            sug="X";
        
        ruleName=trim(ruleName);
        conditionUncut=trim(conditionUncut);
        conclusionUncut=trim(conclusionUncut);
        sug=trim(sug);
        //參考openrule傳入
        int equalIndex=(int)conclusionUncut.find('=');
        string conclusion=conclusionUncut.substr(0,equalIndex-1);
        string shouldBestateString=conclusionUncut.substr(equalIndex+2,1);
        factAdder(conclusion);
        treeAdder(ruleName, conclusion, shouldBestateString,sug,conditionUncut);
    }
    string promName,prom;
    //處理prompt
    if(promNameStartIndex!=-1){
        promStartIndex=(int)todo.find('[',promNameStartIndex);
        promEndIndex=(int)todo.find(']',promNameStartIndex);
        promNameStartIndex=(int)todo.find('T',promNameStartIndex)+1;
        promNameEndIndex=promStartIndex-1;
        prom=todo.substr(promStartIndex,promEndIndex-promStartIndex+1);
        prom=trim(prom);
        promName=todo.substr(promNameStartIndex,promNameEndIndex-promNameStartIndex+1);
        promName=trim(promName);
        promptAdder(promName,prom);
    }
    
    
}

//MAIN- LISTCLAUSE
void listclause(){
    for(int i=0;i<factVec.size();i++){
        string tosay=factVec[i].conclusion;
        tosay.append(" = ");
        tosay.append(factVec[i].state);
        botSay(tosay);
    }
}

//MAIN- ADDCLAUSE
bool addclause(string in){
    string ruleName,statement;
    int equalIndex;
    try{
        equalIndex=(int)in.find('=');
    }catch(exception e){
        botSay("ERROR(FORMAT ERROR)");
        return 0;
    }
    ruleName=in.substr(0,equalIndex);
    statement=in.substr(equalIndex+1,in.size()-equalIndex);
    ruleName=trim(ruleName);
    statement=trim(statement);
    bool chk=0;
    string tosay="The clause [";
    for(int factIndex=0;factIndex<factVec.size();factIndex++){
        if(factVec[factIndex].conclusion==ruleName){
            chk=1;
            tosay.append(ruleName);
            tosay.append("] has changed from ");
            tosay.append(factVec[factIndex].state);
            tosay.append(" to ");
            tosay.append(statement);
            factVec[factIndex].state=statement;
            chk=1;
        }
    }
    if(!chk){
        tosay.append(ruleName);
        tosay.append("] NOT FOUND");
    }
    botSay(tosay);
    return chk;
}

//MAIN- OPENCLAUSE
void openclause(string filepath){
    int count=0;
    char* filepathChar = &filepath[0u];
    int totalLine=countlines(filepathChar);
    for(int i=0;i<totalLine;i++){
        string str=readline(filepathChar, i);
        if(addclause(str))
            count++;
    }
    string ts=filepath;
    ts.append(": ");
    ts.append(to_string(count));
    ts.append(" clauses read\n");
}

int main(){
    string rulepath="/Users/sean/Desktop/dp/A_rules.txt";
    string cmdU,cmd,arg="";
    int spaceIdx;
    while(cin){
        getline(cin, cmdU);
        try {
            if(cmdU.find(' ')!=string::npos){
                spaceIdx=(int)cmdU.find(' ');
                if(cmdU.find_last_of(' ')!=spaceIdx)
                    throw "COMMAND FORMAT ERROR";
                cmd=cmdU.substr(0,spaceIdx);
                arg=cmdU.substr(spaceIdx+1,cmdU.length()-spaceIdx-1);
            }else{
                cmd=cmdU;
            }
            cmd=cmd;
            
        } catch (string e) {
            botSay(e);
        }
    }
    return 0;
}

//HELPER- count lines in file
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

//HELPER- read line from file
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

//HELPER- STRING GET RID OF SPACE
string& trim(std::string & str){
    return ltrim(rtrim(str));
}

//HELPER- STRING GET RID OF LEFT_HAND_SIDE SPACE
string& ltrim(std::string & str){
    auto it2 =  std::find_if( str.begin() , str.end() , [](char ch){ return !std::isspace<char>(ch , std::locale::classic() ) ; } );
    str.erase( str.begin() , it2);
    return str;
}

//HELPER- STRING GET RID OF RIGHT_HAND_SIDE SPACE
string& rtrim(std::string & str){
    auto it1 =  std::find_if( str.rbegin() , str.rend() , [](char ch){ return !std::isspace<char>(ch , std::locale::classic() ) ; } );
    str.erase( it1.base() , str.end() );
    return str;
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
//觀自在菩薩。行深般若波羅蜜多時。照見五蘊皆空。度一切苦厄。舍利子。色不異空。空不異色。色即是空。空即是色。受想行識。亦復如是。舍利子。是諸法空相。不生不滅。不垢不淨。不增不減。是故空中無色。無受想行識。無眼耳鼻舌身意。無色聲香味觸法。無眼界。乃至無意識界。無無明。亦無無明盡。乃至無老死。亦無老死盡。無苦集滅道。無智亦無得。以無所得故。菩提薩埵。依般若波羅蜜多故。心無罣礙。無罣礙故。無有恐怖。遠離顛倒夢想。究竟涅槃。三世諸佛。依般若波羅蜜多故。得阿耨多羅三藐三菩提。故知般若波羅蜜多。是大神咒。是大明咒。是無上咒。是無等等咒。能除一切苦。真實不虛。故說般若波羅蜜多咒。即說咒曰。揭諦揭諦　波羅揭諦　波羅僧揭諦　菩提薩婆訶
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
