#include<iostream>
#include<vector>
#include<ctime>
#include<fstream>
#include<algorithm>
#include<set>
using namespace std;

vector<vector<int> >bipartite_graph;

vector<bool>vx,vy,x_match;
vector<int>y_match;
set<int>left_part,right_part;

vector<int>ans_set;
/*将vector内元素全部设置为0*/
void mem_set_false(vector<bool>& t)
{
    int n = t.size();
    for(int i = 0;i < n;i++)t[i] = false;
}

void read_file(string file_name)
{
    ifstream fin;
    fin.open(file_name,ios::in);
    int n,m;
    fin>>n>>m;
    for(int i = 0;i < n;i++)
    {
        vector<int>temp;
        bipartite_graph.push_back(temp);
        vx.push_back(false);
        vy.push_back(false);
        x_match.push_back(false);
        y_match.push_back(-1);
    }
    for(int i = 0;i < m;i++)
    {
        int a,b;
        fin>>a>>b;
        bipartite_graph[a].push_back(b);
        left_part.insert(a);
        right_part.insert(b);
    }
    //孤立顶点
    for(int i = 0;i < n;i++)
    {
        if(left_part.count(i)==0 && right_part.count(i)==0)
            ans_set.push_back(i);
    }
    fin.close();
}

int find(int x)
{
    if(x == -1)return 0;
    if(x_match[x] == 1) return 0;
    x_match[x] = true;
    int n = bipartite_graph[x].size();
    for(int i = 0;i < n;i++)
    {
        int y = bipartite_graph[x][i];
        if(y_match[y]==-1 || find(y_match[y]))
        {
            y_match[y] = x;
            return 1;
        }
    }
    return 0;
}
/*匈牙利算法*/
int hungary()
{
    int ans = 0;
    int n = bipartite_graph.size();
    for(int i = 0;i < n;i++)
    {
        mem_set_false(x_match);
        ans+=find(i);
    }
    return ans;
}
/*标记最小点覆盖集 左侧部分为被标记的点 右侧被标记的点是最小点覆盖集*/
void mark(int x)
{
    if(vx[x]) return;
    vx[x] = true;
    int n = bipartite_graph[x].size();
    for(int i =0;i < n;i++)
    {
        int y = bipartite_graph[x][i];
        if(y_match[y]>=0 && !vy[y])
        {
            vy[y] = true;
            mark(y_match[y]);
        }
    }
}

int MaxIndependentSet()
{
    int ans = hungary();
    mem_set_false(x_match);
    int m = y_match.size();
    for(int i =0;i < m;i++)
    {
        if(y_match[i]!=-1) x_match[y_match[i]] = true;
    }
    for(int i = 0;i < m;i++)
    {
        if(!x_match[i])mark(i);
    }
    return m - ans;
}
/*用二分图中所有的点减去最小点覆盖集里的点*/
void output(int number)
{
    ofstream fout;
    fout.open("ans.txt",ios::out);
    fout<< "The size of independent set:"<<number<<endl;
    int n = bipartite_graph.size();
    
    for(set<int>::iterator i = left_part.begin();i != left_part.end();i++)
    {
        if(vx[*i]) ans_set.push_back(*i);
    }
    for(set<int>::iterator i = right_part.begin();i != right_part.end();i++)
    {
        if(!vy[*i]) ans_set.push_back(*i);
    }
    sort(ans_set.begin(),ans_set.end());
    for(int i = 0;i < ans_set.size();i++)
    {
        if(i>0 && ans_set[i]==ans_set[i-1]) continue; //错误处理，防止结果中包含孤立点的情况下多添加孤立点
        fout<<ans_set[i]<<endl;
    }
    fout.close();
}

int main(int argc, char const *argv[])
{
    string file_name ;
    cin>>file_name;
    read_file(file_name);
    
    time_t start = clock();
    int ans = MaxIndependentSet();
    time_t end = clock();
    cout << "Computing dataset "<<file_name<<" costs " << (double)(end - start) *1000 / CLOCKS_PER_SEC << " ms" << endl;

    output(ans);
    return 0;
}
