#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <set>
#include <list>
using namespace std;

vector<int> *adjacent_table;
vector<bool> judge; //true表示该结点仍在W中
int n;
list<int> *lengths;
int l_min;
int *remaining_len;

void read_file(string file_name);

void find_MIS();

int delete_nodes(int v);


int main(int argc, const char **argv)
{
    cout << "please type the input file path:" << endl;
    string file;
    cin >> file;
    read_file(file);

    clock_t start = clock();
    find_MIS();
    clock_t end = clock();
    cout << "Compute dataset " << file << " costs " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " ms" << endl;

    delete[] lengths;
    delete[] adjacent_table;
    delete[] remaining_len;
    return 0;
}

void read_file(string file_name)
{
    ifstream fin;
    fin.open(file_name, ios::in);
    if (!fin.is_open())
    {
        std::cout << "Error!" << std::endl;
        exit(1);
    }

    fin >> n;

    remaining_len = new int[n];

    for (int i = 0; i < n; i++)
        judge.push_back(true);
    adjacent_table = new vector<int>[n];
    while (!fin.eof())
    {
        int a, b;
        fin >> a >> b;
        adjacent_table[a].push_back(b);
        adjacent_table[b].push_back(a);
    }
    fin.close();

    vector<int> length;
    for (int i = 0; i < n; i++)
    {
        length.push_back(adjacent_table[i].size());
        remaining_len[i] = adjacent_table[i].size();
    }

    auto max_size = max_element(length.begin(), length.end());
    
    lengths = new list<int>[*max_size + 1]; //初始化的都是空链表
    l_min = *max_size;
    for (int i = 0; i < n; i++)
    {
        lengths[adjacent_table[i].size()].push_front(i);
        if (adjacent_table[i].size() < l_min)
            l_min = adjacent_table[i].size();
    }
}

void find_MIS()
{
    
    int ans = 0;
    int w = n;
    while (w > 0 && !lengths[l_min].empty())
    {
        int v = lengths[l_min].front();
        w = w - delete_nodes(v);
        
        ans++;
    }
    cout << ans << endl;
}

int delete_nodes(int v)
{
    int sum = 1;
    judge[v] = false;
    
    lengths[l_min].pop_front(); //在长度链表中删除v
    for (auto i = adjacent_table[v].begin(); i != adjacent_table[v].end(); i++)
    {
        if (judge[*i])
        {
            int pre_size = remaining_len[*i];
            judge[*i] = false;
            sum++;
            lengths[pre_size].remove(*i);
        }
    }
    while (lengths[l_min].empty())
    {
        l_min++;
    }
    return sum;
}
