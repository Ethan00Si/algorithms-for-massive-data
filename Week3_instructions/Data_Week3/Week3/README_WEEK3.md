


## Tested Environment:（注明实验环境）
- macOS Catallina 10.15.3
- C++ 
- Apple clang version 11.0.3

## Requirements:（程序依赖的工具包，不提倡使用不常见的工具包）
- iostream
- vector
- fstream
- ctime
- list
- set
- algorithm


## Compile the code:
g++ -std=c++11 lab3.cpp -o a.out


## Run:
./a.out [input file name]

eg: ./a.out graph_100.txt graph_10000.txt graph_100000.txt


## Instructions:（额外说明，非必须）
results: ansi.txt对应于第i个读入文件的结果，例如ans1.txt对应第一个输入文件的结果

使用了两种数据结构实现，两者效率不同，在实验报告中具体给出。lab3.cpp和copy.cpp分别对应vector<set<int\>>和vector<vector<int\>\>实现。两者使用方式相同。