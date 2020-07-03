#include"min_hashing.hpp"

const int vocabulary_size = 28102;

min_hash::min_hash(int K):hash_p(28277){
    this->hash_func_nums = K;
    for(int i = 0;i < K;i++){
        default_random_engine random(i+1);
        uniform_int_distribution<int> dis(1,hash_p); //int 随机数范围[1,hash_p]
        hash_a.push_back(dis(random));
        hash_b.push_back(dis(random));
    }
        
}

min_hash::~min_hash(){
}

void min_hash::read_file(string file_name){
    fstream fin;
    fin.open(file_name,ios::in);
    assert(fin.is_open());

    int D,W,NNZ;
    fin>>D>>W>>NNZ;
    for(int i = 0;i < D;i++){ //注意doc ID是从1开始，所以初始化D+1列，第0列不使用
        vector<int>tmp;
        signature.push_back(tmp);
        data.push_back(tmp);
    }
    
    
    for(int i = 0;i < NNZ;i++){
        int a,b,c;
        fin>>a>>b>>c;
        data[a-1].push_back(b);  //从0开始存储
    }

    fin.close();
    
}

void min_hash::gen_signature(){
    for(int i = 0; i < hash_func_nums;i++){
        int size = data.size();
        for(int col = 0; col <  size; col++){
            int min_index = 0x7FFFFFFF;
            for(auto j : data[col]){
                int val = (hash_a[i] * j + hash_b[i]) % hash_p % vocabulary_size;
                if(val < min_index) min_index = val;
            }
            signature[col].push_back(min_index);
        }
    }
}

vector<vector<int> >min_hash::get_signature()const{
    return signature;
} 

vector<vector<int> >min_hash::get_data()const{
    return data;
}