#ifndef LOCALITY_SENSITIVE_HASHING_HPP
#define LOCALITY_SENSITIVE_HASHING_HPP

#include <vector>
#include <iostream>
#include <random>
#include <set>
#include <fstream>

using namespace std;

class LSH{
    private:
        const int b; //b 个 band
        const int r; //r rows per band
        const double s; //阈值s
        vector<vector<int> >min_hash_data;
        int k;// k 个 buckets
        vector<vector<set<int> > >buckets;
        vector<int> hash_a; //b个哈希函数的参数a
        vector<int> hash_b; //b个哈希函数的参数b
        int hash_p; //b个哈希函数的参数p
        vector<set<int> >query_list; //doc i 相似的文档存在vector[i]中的set中
        
    public:
        LSH(int bb, int rr, double ss);
        ~LSH();
        void get_min_hash(vector<vector<int> >data);
        void gennerate_buckets(); //生成buckets并且生成query_list
        set<int>  get_query_list(int index)const;
        void output_query_list(string file_name)const;
};

#endif // !LOCALITY_SENSITIVE_HASHING_HPP
