#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <cassert>
#include <random>
#include <cmath>
#include <queue>
#include <map>
#include <iomanip>
using namespace std;

class discrete_random_variable
{
private:
    vector<int> values;
    vector<pair<double, size_t>> walkers_alias;
    random_device rd_;                                     //随机数引擎
    mt19937 gen_{rd_()};                                   //产生特定分布
    uniform_real_distribution<double> real_dis_{0.0, 1.0}; //real （0，1）上的均匀分布
    uniform_int_distribution<size_t> int_dis_;             //均匀分布，在知道values的size的时候才能够初始化

public:
    discrete_random_variable(const vector<int> &vals, const vector<double> &probs) : values(vals), walkers_alias(generate_walkers_alias(probs)), int_dis_(0, probs.size() - 1)
    {
        assert(vals.size() == probs.size());
        const double sum = accumulate(probs.begin(), probs.end(), 0.0);
        assert(fabs(1.00 - sum) < 0.00000000000001);
    }

    int give_choice()
    {
        const size_t idx = int_dis_(gen_);
        const double proportion = real_dis_(gen_);
        if (proportion > walkers_alias[idx].first && walkers_alias[idx].second != numeric_limits<size_t>::max())
        {
            return values[walkers_alias[idx].second];
        }
        else
        {
            return values[idx];
        }
    }

private:
    vector<pair<double, size_t>> generate_walkers_alias(const vector<double> &probs)
    {
        size_t len = probs.size();
        vector<pair<double, size_t>> walkers_alias_table(len, {0.00, numeric_limits<size_t>::max()});
        queue<size_t> small, large;

        for (size_t i = 0; i < len; i++)
        {
            walkers_alias_table[i].first = probs[i] * len;
            if (walkers_alias_table[i].first > 1.00)
            {
                large.push(i);
            }
            else
            {
                small.push(i);
            }
        }

        while (!small.empty())
        {
            size_t s = small.front(), l = large.front();
            small.pop();
            large.pop();
            walkers_alias_table[s].second = l;
            walkers_alias_table[l].first = walkers_alias_table[l].first - (1.0 - walkers_alias_table[s].first);
            //更新large剩下的部分
            if (walkers_alias_table[l].first < 1.0)
            {
                small.push(l);
            }
            else
            {
                large.push(l);
            }
        }
        return walkers_alias_table;
    }
};

int n, k;
void read_file(vector<int> &values, vector<double> &probs, string file_name);

int main(int argc, const char **argv)
{
    string file_name = "Data_Week7.txt";
    vector<int> values;
    vector<double> probs;
    read_file(values, probs, file_name);
    vector<int>sample(k);
    
    clock_t start = clock();
    discrete_random_variable WAM(values, probs);
    clock_t end  = clock();
    cout << "Preprocess costs " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " ms" << endl;
    
    start = clock();
    for(int i = 0;i < k;i++)sample[i] = WAM.give_choice();
    end = clock();
    cout << "Compute dataset " << file_name << " costs " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " ms" << endl;
    std::cout << "sampling results: " << std::endl;
    for(auto x : sample)std::cout << x << std::endl;
    /*
    map<int, size_t> cnt;
    double times = 300000.0;
    for (size_t i = 0; i != times; ++i)
    {
        int x = WAM.give_choice();
        assert(std::find(values.begin(), values.end(), x) != values.end());
        ++cnt[x];
    }
    cout.precision(6);
    for (auto pair : cnt)
    {
        std::cout << pair.first << "[" << pair.second << "] "<<pair.second / times<<endl;
    }
    */
    return 0;
}

void read_file(vector<int> &values, vector<double> &probs, string file_name)
{
    fstream fin;
    fin.open(file_name, ios::in);
    assert(fin.is_open());

    fin >> n >> k;

    for (int i = 0; i < n; i++)
    {
        int a;
        double b;
        fin >> a >> b;
        values.push_back(a);
        probs.push_back(b);
    }
    
    fin.close();

    double sum_up = accumulate(probs.begin(), probs.end(), 0.00);
    for (int i = 0; i < n; i++)
    {
        probs[i] = probs[i] / sum_up;
    }
}
