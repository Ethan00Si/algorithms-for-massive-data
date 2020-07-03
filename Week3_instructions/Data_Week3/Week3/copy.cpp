#include<iostream>
#include<vector>
#include<fstream>
#include<ctime>
#include<list>
#include<algorithm>
using namespace std;
vector<vector<int> > read_file(string file_name)
{
    ifstream infile;
    infile.open(file_name,ios::in);
    int nums; //number of vertexes
    infile>>nums;
    vector<vector<int> >graph(nums);
    int a = 0,b = 0;
    while( !infile.eof())
    {
        infile >> a >> b;
        graph[a].push_back(b);
    }
    infile.close();
    return graph;
}

bool judge(vector<vector<int> >&G,list<int>ans)
{
    for(list<int>::iterator it = ans.begin();it!=ans.end();it++)
    {
        list<int>::iterator next = it;
        next++;
        if(next == ans.end() )return true;
        if(find(G[*it].begin(),G[*it].end(),*next) == G[*it].end())
        {
            return false;
        }
    }
    return true;//make g++ happy
}

void output(vector<vector<int> >&G,list<int>ans,int i)
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

void DFS_visit(vector<vector<int> >&G,int u,int& time,vector<int>&color,vector<int>&finish,list<int>&ans)
{
    time++;
    int size = G[u].size();
    for(int i = 0;i < size;i++)
    {
        if(color[G[u][i]]==0)
            DFS_visit(G,G[u][i],time,color,finish,ans);
    }
    color[u] = 1;
    time++;
    finish[u] = time;
    ans.push_front(u);
}

void DFS(vector<vector<int> >&G,list<int>&ans)
{
    int size = G.size();
    vector<int>color(size,0);//0 denotes white; 1 denotes black
    //vector<int>pi(size,-1);//-1 denotes NULL. 
    vector<int>finish(size,0);// finish time(u.f)
    int time = 0;
    for(int i = 0;i < size;i++)
    {
        if(color[i] == 0)
        {
            DFS_visit(G,i,time,color,finish,ans);
        }
    }
}

list<int> topological_sort(vector<vector<int> >&G)
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
        vector<vector<int> >graph = read_file(file_name);
        time_t start = clock();
        list<int> res =  topological_sort(graph);
        time_t end = clock();
        cout << "Compute dataset"<<i<<" costs " << (double)(end - start) *1000 / CLOCKS_PER_SEC << " ms" << endl;
        output(graph,res,i);
    }
    return 0;
}
