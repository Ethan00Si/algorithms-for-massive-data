#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <cassert>
#include<string>
using namespace std;
string file_name[3] = {"data_100_10.txt", "data_10000_1000.txt", "data_1e+06_10000.txt"};

int p[3] = {11, 1009, 10007};                                                           //质数p
int a[3][5] = {{2, 3, 5, 7, 9}, {101, 157, 211, 269, 457}, {773, 853, 919, 911, 983}};  //a属于{1,p-1}
int b[3][5] = {{6, 8, 10, 3, 2}, {31, 37, 307, 367, 431}, {613, 673, 1031, 1091, 1601}}; //b属于{0,p-1}
int m[3] = {8, 512, 5023};
int nums[3] = {10, 1000, 10000};
vector<vector<int> > hash_tables;
vector<int> node;
vector<int> change;

void count_min_sketch(int index);
void init(int index);
void output(int index);

int main(int argc, const char **argv)
{
    cout<<"type the index of input file:"<<endl;
    int i;
    cin>>i;
    init(i);
    clock_t start = clock();
    count_min_sketch(i);
    clock_t end = clock();
    cout << "Compute dataset " << file_name[i] << " costs " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " ms" << endl;
    output(i);
    return 0;
}

void init(int index)
{
    vector<int> tmp(m[index], 0);
    for (int i = 0; i < 5; i++)
        hash_tables.push_back(tmp);

    fstream fin;
    fin.open(file_name[index]);
    assert(fin.is_open());

    int a, b;
    while (fin >> a >> b)
    {
        node.push_back(a);
        change.push_back(b);
    }
    fin.close();
}

void count_min_sketch(int index)
{
    int size = node.size();
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            int place = (a[index][j] * node[i] + b[index][j]) % p[index] % m[index];
            hash_tables[j][place] += change[i];
        }
    }
}

void output(int index)
{
    fstream fout;
    string file_out = "ans"+to_string(index)+".txt";
    fout.open(file_out,ios::out);
    int size = nums[index];
    for (int i = 0; i < size; i++)
    {
        int min = 0xfffffff;
        for (int j = 0; j < 5; j++)
        {
            int place = (a[index][j] * i + b[index][j]) % p[index] % m[index];
            if (hash_tables[j][place] < min)
                min = hash_tables[j][place];
        }
        fout << i << " " << min << endl;
    }
}