#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <cassert>
#include <string>
#include <random>
#include <algorithm>
using namespace std;

string file_name[3] = {"data_100_10.txt", "data_10000_1000.txt", "data_1e+06_10000.txt"};
int nums[3] = {10, 1000, 10000};

const int d_cs = 3;           //d个count sketch
int t[3] = {8,512, 5023};    //range{1,2,..,.t}
int p[3] = {11, 1009, 10007}; // u < p <2u
int a[3][d_cs];
int b[3][d_cs];
int c[3][d_cs];
int d[3][d_cs];

vector<vector<int> >  C;
vector<int> node;
vector<int> change;

void init(int index);
void update(int index, int i, int frequency);
int query(int index, int i);
void output(int index);

int main(int argc, const char **argv)
{
    const int index_ = 2;
    init(index_);
    clock_t start = clock();
    int size = node.size();
    for (int i = 0; i < size; i++)
    {
       update(index_, node[i], change[i]);
    }
    clock_t end = clock();
    cout << "Compute dataset " << file_name[index_] << " costs " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " ms" << endl;
   
    output(index_);
    return 0;
}

void init(int index)
{
    //Pick a_j,b_j,c_j,d_j, uniformly from [1...p]
    default_random_engine random(time(NULL));
    uniform_int_distribution<int> dis1(1, p[index]);
    for (int i = 0; i < d_cs; i++)
    {
        a[index][i] = dis1(random);
        b[index][i] = dis1(random);
        c[index][i] = dis1(random);
        d[index][i] = dis1(random);
        //cout<<a[index][i]<<" "<<b[index][i]<<" "<<c[index][i]<<" "<<d[index][i]<<endl;
        C.push_back(vector<int>(t[index]));
    }

    //load data stream
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

void update(int index, int i, int frequency)
{
    for (int j = 0; j < d_cs; j++)
    {

        int h_j_i = (a[index][j] * i + b[index][j]) % p[index] % t[index];
        int g_j_i = 2 * (((c[index][j] * i + d[index][j]) % p[index]) % 2) - 1;
        C[j][h_j_i] += g_j_i * frequency;
        //cout<<i<<" "<<C[j][h_j_i]<<endl;
    }
}

int query(int index, int i)
{
    vector<int> e_j(d_cs);
    for (int j = 0; j < d_cs; j++)
    {
        int h_j_i = (a[index][j] * i + b[index][j]) % p[index] % t[index];
        int g_j_i = 2 * (((c[index][j] * i + d[index][j]) % p[index]) % 2) - 1;
        //default_random_engine random(clock());
        //uniform_int_distribution<int> dis1(0, 1);
        //int g_j_i = dis1(random)*2 - 1 ;
        e_j[j] = g_j_i * C[j][h_j_i];
    }
    sort(e_j.begin(), e_j.end());
    return e_j[e_j.size() / 2];
}

void output(int index)
{
    fstream fout;
    string file_out = "ans"+to_string(index)+".txt";
    fout.open(file_out,ios::out);
    for (int i = 0; i < nums[index]; i++)
    {
        fout << i << " " << query(index, i) << endl;
    }
    fout.close();
}