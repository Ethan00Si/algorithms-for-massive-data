#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>
#include <ctime>
#include <unordered_map>
#include <cassert>
using namespace std;
vector<int> raw_data;
vector<int>elems;
int ittt=0;
int N,k;
void read_file(string file_name,string input_file);

class MG{
private:
    unordered_map<int, int> hash_table;
    vector<pair<int, int> >keys;
    int k;

public:
    MG(int kk){
        k = kk;
    }
    void insert(int element)
    {        
        auto it = hash_table.find(element);
        assert(keys.size() <= k-1);
        //元素在k-1个中
        if (it != hash_table.end())
        {
            keys[it->second].second++;
        }
        else if (keys.size() < k - 1)
        {
            keys.push_back(pair<int, int>(element, 1));
            hash_table.insert(pair<int, int>(element, keys.size()-1));
        }
        else
        {
            for(int i = 0;i<keys.size();i++){
                keys[i].second--;
                if(keys[i].second==0){
                    hash_table.erase(keys[i].first);
                    auto temp = keys.back();
                    keys.back() = keys[i];
                    keys[i] = temp;
                    hash_table[keys[i].first] = i;
                    keys.pop_back();
                    i = i-1;//进行这个被交换过来对元素的更新
                }
            }
        }
    }
    vector<pair<int, int> >get_ans(){
        return this->keys;
    }
};
int main(int argc, char const *argv[])
{
    string file_name = "data_4948603_100.txt";
    string input_file = "in_4948603.txt";
    cout<<"input_file is "<<input_file<<endl;
    read_file(file_name,input_file);
    
    clock_t start = clock();
    MG holder(k);
    for(int i = 0;i < N;i++){
        holder.insert(raw_data[i]);
    }
    clock_t end = clock();
    cout << "Compute dataset " << file_name << " costs " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " ms" << endl;


    vector<pair<int,int> >k_container = holder.get_ans();
    

    vector<int>ans;
    for(auto i : elems){
        int flag = 0;
        for(auto j:k_container){
            if(j.first == i){
                ans.push_back(j.second);
                flag++;break;
            }
        }
        if(!flag)ans.push_back(0);
    }
    for(auto j:ans)cout<<j<<endl;
    return 0;
}

void read_file(string file_name,string input_file)
{
    fstream fin;
    fin.open(file_name);
    assert(fin.is_open());

    while (!fin.eof())
    {
        int t;
        fin >> t;
        raw_data.push_back(t);
    }
    fin.close();

    fin.open(input_file);
    assert(fin.is_open());
    string tmp;
    getline(fin,tmp);
    string num;
    for(int i = 2;i<tmp.size();i++)num += tmp[i];
    N = atoi(num.c_str());
    string tmp2;
    getline(fin,tmp2);
    string num2;
    for(int i = 2;i<tmp.size();i++)num2 += tmp2[i];
    k = atoi(num2.c_str());
    while(!fin.eof()){
        int a;
        fin>>a;
        elems.push_back(a);
    }
    fin.close();
}