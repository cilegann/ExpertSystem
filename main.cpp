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
#include <cctype>
#include <algorithm>
#include <stdio.h>
#include <time.h>
#include <iomanip>


using namespace std;

//函數 全域變數 宣告[v]
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
vector<tree> finalTV;
vector<tree> ordinTV;
vector<statement> statementVec;
bool debug=0,record=0;

//HELP- TIME[v]
class nowtime{
public:
    int year,month,day,hour,min;
    nowtime(){year=1900;month=1;day=1;hour=0;min=0;}
    int returnYear();
    int returnMonth();
    int returnday();
    int returnHour();
    int returnMin();
    void getTime(){
        time_t tt=time(NULL);
        tm* t=localtime(&tt);
        year += t->tm_year;
        month = t->tm_mon+1;
        day = t->tm_mday;
        hour = t->tm_hour;
        min = t->tm_min;
        
    }
    
    void showTime(){
        cout<<"["<<year<<"/"<<setw(2)<<setfill('0')<<month<<"/"<<setw(2)<<setfill('0')<<day;
        cout<<" "<<setw(2)<<setfill('0')<<hour<<":"<<setw(2)<<setfill('0')<<min<<"]";
    }
    
    string returnTime(){
        string str=to_string(year);
        str.append("/");
        str.append(to_string(month));
        str.append("/");
        str.append(to_string(day));
        str.append(" ");
        str.append(to_string(hour));
        str.append(":");
        str.append(to_string(min));
        return str;
    }
    
};

//HELP- BOT SAY[v]
void botSay(string tosay){
    cout<<"BOT> "<<tosay<<endl;
}

//HELP- DEBUG SAY[v]
void debugSay(string tosay){
    cout<<"DEBUG> "<<tosay<<endl;
}

//STRUCT- 事實結構[v]
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

//HELP- FACT ADDER[v]
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

//STRUCT- 小顆狀態結構[v]
struct statement{
    string condition;
    string shouldBeState;
    int ifStateEqual;
    double historyTimes;
    double ratio;
    double Origscore;
    double score;
    statement(){}
    statement(string cond,string shouldbe){
        this->condition=cond;
        this->shouldBeState=shouldbe;
        this->ifStateEqual=-1;
        ratio=1;
        Origscore=1;
        score=1;
        historyTimes=0;
    }
    bool operator==(const statement& rhs) const{
        return (condition == rhs.condition)&&(shouldBeState == rhs.shouldBeState);
    }
    void scoreRefresh(){
        //THERE IS A PARAMETER HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        score=ratio*Origscore*7;
    }
};

//HELP- STATEMENT CHECKER AND ADDER[v]
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

//STRUCT- 邏輯結構，包含名稱(rule name)、sug(suggestion)、result statement*、OR vector<AND vector<狀態結構*in statement vector>>   [v]
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
    
    void writelog(){
        fstream file;
        ofstream log("./logfile.txt",ios_base::app | ios_base::out);
        nowtime time;
        time.getTime();
        log<<time.year<<setw(2)<<setfill('0')<<time.month<<setw(2)<<setfill('0')<<time.day;
        log<<" "<<setw(2)<<setfill('0')<<time.hour<<setw(2)<<setfill('0')<<time.min<<" ";
        log<<name<<": "<<suggestion.substr(1,suggestion.length()-2);
    }
    bool operator<(const tree &rhs) const {
        return resultStatement.score < resultStatement.score;
    }
    
};

//HELP- TREE ADDER [v]
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

//HELP- PROMPT ADDER [v]
void promptAdder(string conclusion,string promptIn){
    for(int i=0;i<factVec.size();i++){
        if(factVec[i].conclusion==conclusion){
            factVec[i].prompt=promptIn;

        }
    }
}

//MAIN- OPENRULE [v]
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

//MAIN- LIST RULE [v]
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

//MAIN- ADDRULE [v]
void addrule(){
    string todo;
    string in;
    cout<<">";
    while(cin){
        getline(cin,in);
        if(in=="@")
            break;
        todo.append(in);
        cout<<">";
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

//MAIN- LISTCLAUSE [v]
void listclause(){
    for(int i=0;i<factVec.size();i++){
        string tosay=factVec[i].conclusion;
        tosay.append(" = ");
        tosay.append(factVec[i].state);
        botSay(tosay);
    }
    cout<<endl;
}

//MAIN- ADDCLAUSE [v]
bool addclause(string in){
    string ruleName,statement,tosay;
    int equalIndex=0,chk=0;
    try{
        equalIndex=(int)in.find('=');
        if(!equalIndex)
            throw new exception;
        tosay="The clause [";
        ruleName=in.substr(0,equalIndex);
        statement=in.substr(equalIndex+1,in.size()-equalIndex);
        ruleName=trim(ruleName);
        statement=trim(statement);
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
    }catch(exception e){
        botSay("ERROR(FORMAT ERROR)");
        return 0;
    }

    botSay(tosay);
    return chk;
}

//MAIN- OPENCLAUSE [v]
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

//HELP- update score from every tree's resultstatement to statementVec(return 1 if at least one value is changed)
bool scoreUpdateToSV(){
    bool changed=0;
    for(int ti=0;ti<treeVec.size();ti++){
        for(int si=0;si<statementVec.size();si++){
            if(statementVec[si]==treeVec[ti].resultStatement){
                if(statementVec[si].Origscore!=treeVec[ti].resultStatement.Origscore){
                    changed=1;
                }
                statementVec[si].ratio=treeVec[ti].resultStatement.ratio;
                statementVec[si].Origscore=treeVec[ti].resultStatement.Origscore;
                statementVec[si].scoreRefresh();
                treeVec[ti].resultStatement.scoreRefresh();
            }
        }
    }
    return changed;
}

//HELP- update score from statementvec to every statement element in treeVec(return 1 if at least one value is changed)
bool scoreUpdateToTV(){
    bool changed=0;
    for(int si=0;si<statementVec.size();si++){
        for(int ti=0;ti<treeVec.size();ti++){
            if(statementVec[si]==treeVec[ti].resultStatement){
                if(statementVec[si].Origscore!=treeVec[ti].resultStatement.Origscore){
                    changed=1;
                }
                treeVec[ti].resultStatement.ratio=statementVec[si].ratio;
                treeVec[ti].resultStatement.Origscore=statementVec[si].Origscore;
                statementVec[si].scoreRefresh();
                treeVec[ti].resultStatement.scoreRefresh();
            }
            for(int oi=0;oi<treeVec[ti].OR.size();oi++){
                for(int ai=0;ai<treeVec[ti].OR[oi].size();ai++){
                    if(statementVec[si]==treeVec[ti].OR[oi][ai]){
                        if(statementVec[si].Origscore!=treeVec[ti].OR[oi][ai].Origscore){
                            changed=1;
                        }
                        treeVec[ti].OR[oi][ai].ratio=statementVec[si].ratio;
                        treeVec[ti].OR[oi][ai].Origscore=statementVec[si].Origscore;
                        statementVec[si].scoreRefresh();
                        treeVec[ti].resultStatement.scoreRefresh();
                    }
                }
            }
        }
    }
    return changed;
}

//HELP- read log to assign initial score [v]
void readlog(){
    string filepath="/Users/sean/Desktop/dp/code/log.txt";
    char* filepathChar=&filepath[0u];
    double totalLine=countlines(filepathChar);
    for(double line=0;line<totalLine;line++){
        string str=readline(filepathChar, line);
        int spindex=(int)str.find(' ');
        spindex=(int)str.find(' ',spindex+1);
        int endindex=(int)str.find('_');
        string statename=str.substr(spindex,endindex-spindex);
        statename=trim(statename);
        for(int ti=0;ti<treeVec.size();ti++){
            if(treeVec[ti].name==statename){
                for(int stindex=0;stindex<statementVec.size();stindex++){
                    if(statementVec[stindex]==treeVec[ti].resultStatement){
                        statementVec[stindex].historyTimes++;
                    }
                }
            }
        }
    }
    for(int treeindex=0;treeindex<treeVec.size();treeindex++){
        for(int stateindex=0;stateindex<statementVec.size();stateindex++){
            if(treeVec[treeindex].resultStatement==statementVec[stateindex]){
                treeVec[treeindex].resultStatement.historyTimes=statementVec[stateindex].historyTimes;
                treeVec[treeindex].resultStatement.ratio=2-(double)(statementVec[stateindex].historyTimes/totalLine);
                scoreUpdateToSV();
            }
        }
    }
}

//HELP- update ifequal from tree's result statement to statementVec
bool stateUpdateToSV(){
    bool changed=0;
    for(int ti=0;ti<treeVec.size();ti++){
        for(int si=0;si<statementVec.size();si++){
            if(statementVec[si]==treeVec[ti].resultStatement){
                if(statementVec[si].ifStateEqual!=treeVec[ti].resultStatement.ifStateEqual){
                    changed=1;
                }
                statementVec[si].ifStateEqual=treeVec[ti].resultStatement.ifStateEqual;
            }
        }
    }
    return changed;
}

//HELP- update statementVec's statment ifEqual by reading facts
bool stateUpdateToTV(){
    bool changed=0;
    for(int si=0;si<statementVec.size();si++){
        for(int ti=0;ti<treeVec.size();ti++){
            if(statementVec[si]==treeVec[ti].resultStatement){
                if(statementVec[si].ifStateEqual!=treeVec[ti].resultStatement.ifStateEqual){
                    changed=1;
                }
                treeVec[ti].resultStatement.ifStateEqual=statementVec[si].ifStateEqual;
            }
            for(int oi=0;oi<treeVec[ti].OR.size();oi++){
                for(int ai=0;ai<treeVec[ti].OR[oi].size();ai++){
                    if(statementVec[si]==treeVec[ti].OR[oi][ai]){
                        if(statementVec[si].ifStateEqual!=treeVec[ti].OR[oi][ai].ifStateEqual){
                            changed=1;
                        }
                        treeVec[ti].OR[oi][ai].ifStateEqual=statementVec[si].ifStateEqual;
                    }
                }
            }
        }
    }
    return changed;
}

//HELP- updateSV state from fV
bool updateSVIFfromFV(){
    bool changed=0;
    for(int fi=0;fi<factVec.size();fi++){
        for(int si=0;si<statementVec.size();si++){
            if(statementVec[si].condition==factVec[fi].conclusion){
                if(factVec[fi].state!="?"){
                    if(statementVec[si].shouldBeState==factVec[fi].state){
                        statementVec[si].ifStateEqual=1;
                    }else{
                        statementVec[si].ifStateEqual=0;
                    }
                    statementVec[si].Origscore=0;
                    statementVec[si].scoreRefresh();
                }
            }
        }
    }
    stateUpdateToTV();
    scoreUpdateToTV();
    return changed;
}

//MAIN- INFERENCE
void inf(){
    readlog();
    updateSVIFfromFV();
    //自我判斷直到沒有改變（若最終條件已知不成立，score變負）
    //tree排序
    //輪流prompt，每prompt一次就自我判斷一輪，再排序
}

int main(){
    
    //string rulepath="/Users/sean/Desktop/dp/A_rules.txt";
    string cmdU,cmd,arg="";
    int spaceIdx;
    cout<<"> ";
    while(cin){
        getline(cin, cmdU);
        cmd="";
        arg="";
        try {
            if(cmdU.find(' ')!=string::npos){
                spaceIdx=(int)cmdU.find(' ');
                if(cmdU.find_last_of(' ')!=spaceIdx){
                    string e="COMMAND FORMAT ERROR";
                    throw e;
                }
                cmd=cmdU.substr(0,spaceIdx);
                arg=cmdU.substr(spaceIdx+1,cmdU.length()-spaceIdx-1);
            }else{
                cmd=cmdU;
            }
            transform(cmd.begin(), cmd.end(), cmd.begin(), (int(*)(int))tolower);
            cmd=trim(cmd);
            arg=trim(arg);
            if(cmd=="openrule"){
                openrule(arg);
            }else if(cmd=="listrule"){
                listrule();
            }else if(cmd=="addrule"){
                addrule();
            }else if(cmd=="addclause"){
                addclause(arg);
            }else if(cmd=="openclause"){
                addclause(arg);
            }else if(cmd=="listclause"){
                listclause();
            }else if(cmd=="inference"){
                inf();
            }else if(cmd=="debugon"){
                debug=1;
            }else if(cmd=="debugoff"){
                debug=0;
            }else if(cmd=="recordon"){
                record=1;
            }else if(cmd=="recordoff"){
                record=0;
            }else if(cmd=="quit"){
                botSay("GoodBye!");
                break;
            }else{
                string a="CMD ERROR";
                throw a;
            }
        } catch (string e) {
            e.append(".\nCMDs: OPENRULE [arg]/LISTRULE/ADDRULE/ADDCLAUSE [arg]/OPENCLAUSE/LISTCLAUSE/INFERENCE/DEBUGON/DEBUGOFF/RECORDON/RECORDOFF/QUIT\n");
            botSay(e);
        }
        cout<<"> ";
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
