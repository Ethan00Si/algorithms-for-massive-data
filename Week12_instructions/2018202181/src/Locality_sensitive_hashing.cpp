#include "Locality_sensitive_hashing.hpp"

const int bucket_size = 2000000;
LSH::LSH(int bb,int rr,double ss):b(bb),r(rr),s(ss){
    this->k = bucket_size;
    for(int i = 0;i < b;i++){  //每一个band对应一个bucket，总共b个buckets
        vector<set<int> > tmp(k);
        buckets.push_back(tmp);
    }

    hash_p = 2000003; //
    //产生b个哈希函数
    for(int i = 0;i < b*r;i++){
        default_random_engine random(i+1);
        uniform_int_distribution<int> dis(1,hash_p); //int 随机数范围[1,hash_p]
        hash_a.push_back(dis(random));
        hash_b.push_back(dis(random));
    }

   
}

LSH::~LSH(){
}

void LSH::get_min_hash(vector<vector<int> >data){
    min_hash_data = data;
}

void LSH::gennerate_buckets(){
    int size = min_hash_data.size();
    for(int i = 0;i < this->b;i++){
        for(int col = 0; col < size;col++){
            size_t sum = 0;
            size_t seed = 7;
            for(int j = 0;j < this->r;j++){
                sum = sum * seed + min_hash_data[col][i*r + j]; //参考了BKDR Hash算法   
            }
            int bucket_index = sum % bucket_size;
            buckets[i][bucket_index].insert(col);
        }
    }
    //初始化query-list
    set<int>tmp;
    for(int  i= 0;i < size;i++) query_list.push_back(tmp);
    

    for(auto bucket : buckets){
        for(auto slot : bucket){
            if(slot.size() < 2)continue;
            for(auto elem = slot.begin();elem != slot.end();elem++){
                for(auto other_elem = elem; other_elem!=slot.end();other_elem++){
                    if(other_elem == elem)continue;
                    query_list[*elem].insert(*other_elem);
                    query_list[*other_elem].insert(*elem);
                }
            }
        }
    }
    
}

set<int>  LSH::get_query_list(int index)const{
    return query_list[index];
}

void LSH::output_query_list(string file_name)const{
    fstream fout;
    fout.open(file_name,ios::out);
    if(!fout.is_open()){
        cout<<"cannot open the file"<<endl;
        exit(1);
    }
    
    int size = query_list.size();
    fout<<"Doc ID : similar Docs"<<endl;
    fout<<"如果Doc ID没有相似文档，冒号后无内容"<<endl;
    for(int i = 0;i < size;i++){
        fout<<to_string(i+1)<<" : ";
        for(auto x : query_list[i]){
            fout<<to_string(x+1)<<" ";
        }fout<<endl;
    } 
    fout.close();
}