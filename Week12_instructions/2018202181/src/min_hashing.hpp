#ifndef MIN_HASHING_HPP
#define MIN_HASHING_HPP

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include <random>
using namespace std;

class min_hash
{
private:
    int num_Docs;                  //number of documents
    int hash_func_nums;            //min hash的hash func的个数
    vector<vector<int> > signature; //存储signature
    vector<vector<int> > data;
    const int hash_p ; //hash func的参数 (ax+b)mod p mod N 中的p，选择一个大于N的prime number，测试样例中N=28102，所以可以选择28277
    vector<int> hash_a; //hash func参数a
    vector<int> hash_b; //hash func 参数b

public:
    min_hash(int K);
    ~min_hash();
    void read_file(string file_name);
    void gen_signature(); //生成signature
    vector<vector<int> > get_signature() const;
    vector<vector<int> > get_data() const;
};

#endif // !MIN_HASHING_HPP
