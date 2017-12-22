# SimpleExpert System in c++

Final Project of Discrete Mathematics, NTU ESOE, 2017 spring


## Getting Started


### Usage

```
g++ main.cpp -o es.out
./es.out
```

System will adjust the order of prompting based on log file and current facts.<br>
Commands are:<br>
0.  QUIT<br>
1.  OPENRULE [rule file path]- Read rules（Reference: A_rules.txt）<br>
2.  LISTRULE- System will list rules<br>
3.  ADDRULE- Add rule to system manually<br>
4.  OPECLAUSE [fact file path]- Read known facts file（Reference: fact_example.txt）<br>
5.  LISTCLAUSE- List facts<br>
6.  ADDCLAUSE- Add facts manually<br>
7.  DEBUGON/DEBUGOFF- Whether the system will display the detail while inferencing or not. <br>
8.  RECORDON/RECORDOFF- To record the result to logfile or not.<br>
9.  INFERENCE- Inferencing based on currencct facts, and prompt if needed.<br>


此為以c++編寫之簡易專家系統
會根據歷史以及現有事實調整詢問順序
指令為

0.  QUIT
1.  OPENRULE [rule file path]讀入規則（格式參考A_rules.txt）
2.  LISTRULE 系統列出整理過後之規則
3.  ADDRULE 手動加入規則 
4.  OPECLAUSE [fact file path]讀入已知事實（格式參考fact_example.txt）
5.  LISTCLAUSE 列出現有事實
6.  ADDCLAUSE 手動加入事實子句
7.  DEBUGON/DEBUGOFF 開啟/關閉推論時之詳細訊息
8.  RECORDON/RECORDOFF 開啟/關閉得到結論後是否紀錄於log檔
9.  INFERENCE 針對現有事實進行推論，系統會依據需求提問
