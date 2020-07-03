#include"min_hashing.hpp"
#include"Locality_sensitive_hashing.hpp"

#include <ctime>

double JS(vector<vector<int> >&data,int doc1,int doc2){
    set<int>set_1,set_2;
    
    for(auto i:data[doc1])set_1.insert(i);
    for(auto i:data[doc2])set_2.insert(i);
    
    
    int union_num = 0;
    int itersection_num = 0;
    for(auto i:set_1){
        if(set_2.find(i) != set_2.end())  itersection_num++;
    }
    union_num = set_1.size()+set_2.size()-itersection_num;
    return (double)itersection_num/(double)union_num;
}

int main(int argc, const char** argv) {
    
    min_hash test1(256);// epsilon = 0.15 delta = 0.1
    test1.read_file("./docword.enron.txt");

    time_t start = clock();

    test1.gen_signature();
    vector<vector<int> > data = test1.get_data();
    vector<vector<int> > sig = test1.get_signature();
    
    LSH test2(8,32,0.93);  //0.93 = (1/8)^(1/32)
    test2.get_min_hash(sig);
    test2.gennerate_buckets();

    time_t end = clock();
    cout <<"Min-hash and LSH costs " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " ms" << endl<<endl;

    test2.output_query_list("LSH_buckets.txt");


    cout<<"query and calculate Jaccard Similarity"<<endl;
    int queries[6] = {0,92,5083,7366,14087,29556};
    for(int i = 0;i < 6;i++){
        set<int>ans = test2.get_query_list(queries[i]);
        cout<<"query Doc ID "<<queries[i]+1<<" :"<<endl;
        for(auto x : ans){
            cout<<"doc "<<x<<" Jaccard Similarity: ";
            cout<<JS(data,queries[i],x)<<endl;
        }cout<<endl;
    }
    return 0;
}