#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<list>
#include<algorithm>
#include<ctime>
using namespace std;

vector<int>* S; //每一个集合对应结点
vector<int>set_length;
vector<bool>covered;//统计每一个元素是否以及covered
vector<vector<int> >node_set;//每一个结点对应的所有集合
vector<list<int> >L;//维护 对应大小的集合 L[k]中均是大小为k的集合
int L_max;
int n,m; // n 元素数 m 集合数

void read_file(string file_name);

vector<int> greedy_set_cover();

void decrease(int vertex);
void update(int vertex);

void output(vector<int>ans);

int main(int argc, const char** argv) {
    string file_name;
    std::cout << "please type the file path, absolute path and relative path are both OK." << std::endl;
    cin>>file_name;

    //read_file
    read_file(file_name);

    clock_t start = clock();
    vector<int > ans = greedy_set_cover();
    clock_t end = clock();
    cout << "Compute dataset "<<file_name<<" costs " << (double)(end - start) *1000 / CLOCKS_PER_SEC << " ms" << endl;

    output(ans);

    delete [] S;
    return 0;
}

void read_file(string file_name)
{
    ifstream fin;
    fin.open(file_name,ios::in);
    if(!fin.is_open())
    {
        std::cout << "Error!" << std::endl;
        exit(1);
    }
    //int n,m;
    fin>>n>>m;
    fin.get(); //去掉换行符 否则getline会少读一行

    S = new vector<int> [m];

    for(int i = 0;i < m;i++)
    {
        string tmp;
        getline(fin,tmp);
        stringstream line(tmp);
        string node;
        while(line>>node)
        {
            S[i].push_back(atoi(node.c_str()));
        }
        set_length.push_back(S[i].size());
        
    }

    fin.close();

    for(int i = 0;i < n;i++)
    {
        covered.push_back(false);
        vector<int>tmp;
        node_set.push_back(tmp);
    }

    for(int i = 0;i < m;i++)
    {
        for(auto j : S[i])
        {
            node_set[j].push_back(i);
        }
    }

    auto max = max_element(set_length.begin(),set_length.end());
    
    vector<list<int> >temporay( *max + 1 ); //set的大小从1开始，所以加1
    L_max = *max;
    for(int i = 0;i < set_length.size();i++)
    {
        temporay[ set_length[i] ].push_back(i);
    } 
    L = temporay;
    
}

vector<int> greedy_set_cover()
{
    vector<int> res;
    while(n > 0)
    {
        n = n - L_max;

        res.push_back(L[L_max].front());
        
        update(L_max);
    }

    return res;
}


void update(int set_size)
{
    int index = L[set_size].front();
    L[set_size].pop_front();

    for(auto vertex : S[index])
    {
        decrease(vertex);
    }

    while(L_max>=0&&L[L_max].empty())
    {
        L_max--;
    }
}

/*更新L中所有集合的大小*/
void decrease(int vertex)
{
    if(covered[vertex]) return;

    covered[vertex] = true;

    for(auto S_i : node_set[vertex])
    {
        for(list<int>::iterator l = L[ set_length[S_i] ].begin();l != L[ set_length[S_i] ].end();l++)
        {
            if(*l == S_i) 
            {
                L[ set_length[S_i] ].erase(l);
                break;
            }
        }
        set_length[S_i] --;
        L[ set_length[S_i] ].push_back(S_i);
    }
}

void output(vector<int>ans)
{
    ofstream fout;
    fout.open("ans.txt",ios::out);

    fout<<ans.size()<<endl;
    for(auto i : ans)
    {
        for(auto j : S[i])
        {
            fout<<j<<" ";
        }
        fout<<endl;
    }

    fout.close();
}