#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include "BloomFilter.hpp"
#include <string>
#include <cassert>
using std::fstream;
using std::string;
using std::vector;

string file_name[2] = {"../Week11_Data/f100.txt","../Week11_Data/f10000.txt"};
string query_name[2] = {"../Week11_Data/q100.txt","../Week11_Data/q10000.txt"};

vector<bool>answer_1e4;
vector<string>data_flow;
vector<string>query;
void read_file();
void write_to_file(vector<bool>&ans);
void write_false_postive(vector<size_t>&ans);

int main(int argc, const char **argv)
{
    read_file();
    Bloomfilter BF();
    for(auto i:data_flow)BF.insert(i);
    vector<bool>ans;
    for(auto i:query)ans.push_back(BF.contains(i));
    write_to_file(ans);
    /*
    vector<size_t>count_FP;
    for(int i = 0;i < 66;i++){
        int m = 20000 + 1000*i;
        int k = (m / 10000.0)*0.69314718;// ln2 =  0.69314718
        if(k<=1)k = 1;
        Bloomfilter BF(k,m);
        for(auto i:data_flow)BF.insert(i);
        vector<bool>ans;
        for(auto i:query)ans.push_back(BF.contains(i));
        //write_false_postive(ans,m);
        int size = answer_1e4.size();
        size_t count = 0;
        for(int i = 0;i<size;i++)a
        {
            if(answer_1e4[i]!=ans[i])count++;
        }
        count_FP.push_back(count);
    }
    write_false_postive(count_FP);*/
    return 0;
}

void read_file()
{
    int index = 1;
    fstream fin;
    fin.open(file_name[index]);
    assert(fin.is_open());

    while(!fin.eof()){
        string line;
        std::getline(fin,line);
        data_flow.push_back(line);
    }
    fin.close();

    fin.open(query_name[index]);
    assert(fin.is_open());

    while(!fin.eof()){
        string line;
        std::getline(fin,line);
        query.push_back(line);
    }
    fin.close();

    fin.open("../Week11_Data/a10000.txt");
    assert(fin.is_open());
    while(!fin.eof()){
        bool ans;
        fin>>ans;
        answer_1e4.push_back(ans);
    }
    fin.close();

}

void write_to_file(vector<bool>&ans)
{
    fstream fout;
    fout.open("answer.txt",std::ios::out);
    assert(fout.is_open());
    for(auto i:ans){
        fout<<i<<std::endl;
    }
    fout.close();
}

void write_false_postive(vector<size_t>&ans){
    fstream fout;
    fout.open("accuracy.csv",std::ios::out);
    assert(fout.is_open());

    int size = ans.size();
    for(int i = 0;i < size;i++){
        int m = 20000 + 1000*i;
        fout<<m<<","<<ans[i]<<std::endl;
    }
}