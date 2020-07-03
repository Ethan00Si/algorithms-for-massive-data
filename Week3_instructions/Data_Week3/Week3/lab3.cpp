#include<iostream>
#include<vector>
#include<fstream>
#include<ctime>
#include<list>
#include<set>
#include<algorithm>
using namespace std;
vector<set<int> > read_file(string file_name)
{
    ifstream infile;
    infile.open(file_name,ios::in);
    int nums; //number of vertexes
    infile>>nums;
    vector<set<int> >graph(nums);
    int a = 0,b = 0;
    while( !infile.eof())
    {
        infile >> a >> b;
        graph[a].insert(b);
    }
    infile.close();
    return graph;
}

bool judge(vector<set<int> >&G,list<int>ans)
{
    for(list<int>::iterator it = ans.begin();it!=ans.end();it++)
    {
        list<int>::iterator next = it;
        next++;
        if(next == ans.end() )return true;
        if(!G[*it].count(*next) )
        {
            return false;
        }
    }
    return true;//make g++ happy
}

void output(vector<set<int> >&G,list<int>ans,int i)
{
    //redirect
    string file_name = "ans" + to_string(i)+".txt";
    ofstream fout;
    fout.open(file_name,ios::out);

    if(judge(G,ans) == 0)
    {
        fout<<0<<endl;
        
        return;
    }
    fout<<1<<endl;
    for(list<int>::iterator it = ans.begin();it != ans.end();it++)
    {
        fout << *it<<' ';
    }
    fout.close();
    return;
}

void DFS_visit(vector<set<int> >&G,int u,vector<int>&color,list<int>&ans)
{
    int size = G[u].size();
    for(auto it:G[u])
    {
        if(color[it] == 0)
            DFS_visit(G,it,color,ans);
    }
    color[u] = 1;

    ans.push_front(u);
}

void DFS(vector<set<int> >&G,list<int>&ans)
{
    int size = G.size();
    vector<int>color(size,0);//0 denotes white; 1 denotes black

    for(int i = 0;i < size;i++)
    {
        if(color[i] == 0)
        {
            DFS_visit(G,i,color,ans);
        }
    }
}

list<int> topological_sort(vector<set<int> >&G)
{
    list<int> ans;
    DFS(G,ans);
    return ans;
}

int main(int argc, char const *argv[])
{
    for(int i = 1;i < argc;i++)
    {
        string file_name = argv[i];
        vector<set<int> >graph = read_file(file_name);
        time_t start = clock();
        list<int> res =  topological_sort(graph);
        time_t end = clock();
        cout << "Compute dataset"<<i<<" costs " << (double)(end - start) *1000 / CLOCKS_PER_SEC << " ms" << endl;
        output(graph,res,i);
    }
    return 0;
}
