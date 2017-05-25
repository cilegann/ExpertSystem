

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <vector>

using namespace std;

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


void writeNewTXT_old(string stringIn){
    fstream file;
    file.open("./rules_sub.txt",ios::app);
    
    long pre,start,end;
    string result;
    pre=stringIn.find_first_of("(");
    start=stringIn.find_last_of("(");
    if(stringIn[pre+1]=='r'){
        result=stringIn.substr(pre+6,stringIn.length()-pre-6);
        file<<"R,"<<"("<<result<<")"<<endl;
    }
    else if(stringIn[pre+1]=='i'){
        end=stringIn.find_first_of(")");
        result=stringIn.substr(start+1,end-start-1);
        file<<"F,"<<"("<<result<<")"<<endl;
    }else if(stringIn[pre+1]=='t'){
        end=stringIn.find_first_of(")");
        result=stringIn.substr(start+1,end-start-1);
        file<<"M,"<<"("<<result<<")"<<endl;
    }else if(stringIn[pre+1]=='a'){
        end=stringIn.find_first_of(")");
        result=stringIn.substr(pre+1,end-pre-1);
        file<<"F,"<<"("<<result<<")"<<endl;
    }
    file.close();
}

void writeNewTXT(string stringIn){
    fstream file;
    file.open("./rules_sub.txt",ios::app);
    
    long pre,start,end;
    string result;
    pre=stringIn.find_first_of("(");
    start=stringIn.find_last_of("(");
    if(stringIn[pre+1]=='r'){
        result=stringIn.substr(pre+6,stringIn.length()-pre-6);
        file<<"R,"<<"("<<result<<")"<<endl;
    }
    else if(stringIn[pre+1]=='i'){
        end=stringIn.find_first_of(")");
        result=stringIn.substr(start+1,end-start-1);
        file<<"F,"<<"("<<result<<")"<<endl;
    }else if(stringIn[pre+1]=='t'){
        end=stringIn.find_first_of(")");
        result=stringIn.substr(start+1,end-start-1);
        file<<"M,"<<"("<<result<<")"<<endl;
    }else if(stringIn[pre+1]=='a'){
        end=stringIn.find_first_of(")");
        result=stringIn.substr(pre+1,end-pre-1);
        file<<"F,"<<"("<<result<<")"<<endl;
    }
    file.close();
}

int increment(int *n,int lines) {
    *n = *n + 1;
    return *n;
}

void countting(int &R,int &F,int &M,int lines){

    for(int i=0;i<lines;i++){
        string todo=readline("rules_sub.txt",i);
        if(todo[0]=='R'){
            R++;
        }else if(todo[0]=='I'){
            F++;
        }else if(todo[0]=='M'){
            M++;
        }
    }
}

string getInfo(string In){
    return In.substr(2,In.length()-2);
}

int main() {
    char* filename="A_rules.txt";
    remove("./rules_sub.txt");
    int lines=countlines(filename);
    for(int i=1;i<lines;i++){
        writeNewTXT(readline(filename,i));
    }
    int numR=0,numF=0,numM=0,numH=0;
    countting(numR,numF,numM,lines);
    vector<string> R;
    vector<string> F;
    vector<string> M;
    vector<string> H;
    lines=countlines("rules_sub.txt");
    for(int i=0;i<lines;i++){
        string str=readline("rules_sub.txt", i);
        if(str[0]=='R'){
            R.push_back(getInfo(str));
        }
        if(str[0]=='F'){
            F.push_back(getInfo(str));
        }
        if(str[0]=='M'){
            H.push_back(getInfo(str));
        }
    }
    for(int i=0; i<F.size(); i++){
        for(int j=0;j<H.size();j++){
            if(F[i]==H[j]){
                bool chk=1;
                for(int k=0;k<M.size();k++){
                    if(M[k]==F[i]){
                        chk=0;
                    }
                }
                if(chk){
                    M.push_back(F[i]);
                }

            }
        }
    }
    for(int i=0; i<F.size(); i++){
        for(int j=0;j<M.size();j++){
            if(F[i]==M[j]){
                F.erase(F.begin()+i);
                i--;
            }
        }
    }
    for(int i=0; i<H.size(); i++){
        for(int j=0;j<M.size();j++){
            if(H[i]==M[j]){
                H.erase(H.begin()+i);
                i--;
            }
        }
    }
    for(int i=0; i<F.size(); i++){
        for(int j=i+1;j<F.size();j++){
            if(F[i]==F[j]){
                F.erase(F.begin()+i);
                i--;
            }
        }
    }

    
    for(int i=0;i<F.size();i++){
        cout<<"F["<<i+1<<"]  "<<F[i]<<endl;
    }
    for(int i=0;i<M.size();i++){
        cout<<"M["<<i+1<<"]  "<<M[i]<<endl;
    }
    for(int i=0;i<H.size();i++){
        cout<<"H["<<i+1<<"]  "<<H[i]<<endl;
    }


    
    return 0;
}
