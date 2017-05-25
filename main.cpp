#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

//函數宣告
int countlines(char*);
string readline(char*,int);


//狀態結構，包含描述與TF
struct statement{
    string discription;
    bool state;
};

//邏輯小結構，包含描述(discreption)、編號(num)、sug(suggestion)、OR vector<AND vector<狀態結構>>
class tree{
private:
    int num;
    string name;
    string suggestion;
    string conclusion;
    vector<vector<vector<statement > > > OR;
public:
    tree();
    tree(int,string,string);
    void printSug(){
        cout<<"BOT>"<<suggestion;
    }
};

//事實結構
struct facts{
    char type;
    statement stats;
    facts(char t,string n,bool s){
        type=t;
        stats.discription=n;
        stats.state=s;
    }
    string prompt;
};

vector<tree> treeVec;
vector<facts> factVec;

void treeAdder(vector<tree>& treeVec,string ruleName,string conditon,string conclution,string sug){
    //treeAdder with sug
}

void treeAdder(vector<tree>& treeVec,string ruleName,string conditon,string conclution){
    //treeAdder without sug
}

void factAdder(vector<facts>& factVec,string statement){
    
}

//openRule
void openrule(string filepath,vector<tree>& treeVec,vector<facts>& factVec){
    char* filepathChar=&filepath[0u];
    int totalLine=countlines(filepathChar);
    //找rule所在行（全部，並記錄全部所在行）
    for(int i=0;i<totalLine;i++){
        string str=readline(filepathChar, i);
        if(str[0]=='R'){
            int ruleLine=i;
            string ruleName=str;
            string condition=readline(filepathChar,++ruleLine);
            string conclusion=readline(filepathChar,++ruleLine);
            string sug;
            string tmp=readline(filepathChar, ++ruleLine);
            if(tmp[0]=='S'){
                sug=readline(filepathChar, ++ruleLine);
                treeAdder(treeVec, ruleName, condition, conclusion,sug);
            }else{
                treeAdder(treeVec, ruleName, condition, conclusion);
            }
            
        }
        
        
    }
    //依據個數跑迴圈
    //treevec 新增一個元素
    //factvec 新增一個元素
    
}


int main(){

    string command,parameter;
    
    cout<<"OPENRULE/LISTRULE/ADDRULE/OPENCLAUSE/LISTCLAUSE/ADDCLAUSE/DEBUG/QUIT : ";
    cin>>command;
    

    while(command!="QUIT"){
        if(command=="OPENRULE"){
            cin>>parameter;
        //呼叫openrule function
            openrule(parameter, treeVec, factVec);
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
