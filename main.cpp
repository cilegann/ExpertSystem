#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

//函數宣告
int countlines(char*);
string readline(char*,int);




//事實結構
struct fact{
    string conclusion;
    string state;
    string prompt;
    fact();
    fact(string conclusion){
        this->conclusion=conclusion;
        this->state="N";
    }
    
};

struct statement{
    string conclusion;
    string souldBeState;
    bool ifStateEqual;
    statement();
    statement(string conclu,string souldbe){
        this->conclusion=conclu;
        this->souldBeState=souldbe;
        this->ifStateEqual=0;
    }
};
//邏輯小結構，包含描述(rule name)、sug(suggestion)、cOR vector<AND vector<狀態結構>>
class tree{
public:
    string name;
    string suggestion;
    string conclusion;
    vector<vector<statement* > >  OR;
    tree();
    tree(string name,string conclusion){
        this->name=name;
        this->conclusion=conclusion;
    }
    
    tree(string name,string conclusion,string suggestion){
        this->name=name;
        this->conclusion=conclusion;
        this->suggestion=suggestion;
    }
    void printSug(){
        cout<<"BOT>"<<suggestion;
    }
};



set<tree*> treeSet;
set<fact*> factSet;
set<statement*> statementSet;
void treeAdder(string ruleName,string conclution,string suggestion,string ifUncut){
    tree* t=new tree(ruleName,conclution,suggestion);
    vector<statement*> andVec;
    int numOfCondition=1;
    for(int i=0;i<ifUncut.length();i++){
        if(ifUncut[i]=='&'){
            numOfCondition++;
        }
    }
    string conditionUncut;
    string condition;
    string shouldBecondition;
    if(numOfCondition==1){
        
    }else{
        
    }
    statement* state=new statement();
    
    
}



void factAdder(string conclu){
    fact* f=new fact(conclu);
    factSet.insert(f);
}

//openRule
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
            treeAdder(ruleName, conclusion, suggestion,ifStringUncut);
            
        }
    }
    
}


int main(){
    openrule("/Users/sean/Desktop/dp/A_rules.txt");
    /*
    string command,parameter;
    
    cout<<"OPENRULE/LISTRULE/ADDRULE/OPENCLAUSE/LISTCLAUSE/ADDCLAUSE/DEBUG/QUIT : ";
    cin>>command;
    

    while(command!="QUIT"){
        if(command=="OPENRULE"){
            cin>>parameter;
        //呼叫openrule function
            openrule(parameter);
        }else if(command=="LISTRULE"){
        //呼叫列印規則函式
        }else if(command=="ADDRULE"){
        //new sub結構
        }else if(command=="OPENCLAUSE"){
        //呼叫讀事實函式（傳入事實結構）
        }else if(command=="LISTCLAUSE"){
        //呼叫列印事實結構
        }else if(command=="ADDCLAUSE"){
        //new sub事實結構
        }else if(command=="DEBUG"){
        //??????????
        }
        else{
            cout<<"Command Not Found."<<endl;
        }
        cout<<"OPENRULE/LISTRULE/ADDRULE/OPENFACT/LISTCLAUSE/ADDCLAUSE/DEBUG/QUIT";
        cin>>command;
    }
     */
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
