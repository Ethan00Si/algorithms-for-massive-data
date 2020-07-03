#ifndef BLOOMFILTER_HPP
#define BLOOMFILTER_HPP

#include <openssl/md5.h>
#include <vector>
#include <string>

class Bloomfilter
{
private:
    //MD5 hash result,总是16bytes大小
    static const size_t MD5_result_size_bytes = 16;

    //bloomfilter storage in bits 2^16 = 65536;
    const size_t bloomfilter_store_size ;

    const size_t hash_function_count;

	std::vector<bool>bloomfilter_store_;
    size_t object_count_;
    unsigned char* MD5_hash_result_buffer;
	
    void hash(const std::string& val)const
    {
        //强制类型转换，仅仅是复制了bits, string.data后无\0和.c_string()相比
		const unsigned char* const md5_input_val = reinterpret_cast<const unsigned char*>(val.data());
		const size_t md5_input_length = val.length();
		MD5(md5_input_val, md5_input_length, MD5_hash_result_buffer);
    }

public:
    Bloomfilter(size_t hash_func_count = 4,size_t bloom_storage_size = 65536): hash_function_count(hash_func_count),
    object_count_(0),MD5_hash_result_buffer(new unsigned char [MD5_result_size_bytes]), bloomfilter_store_size(bloom_storage_size)
    {
        if(MD5_result_size_bytes < hash_function_count * 2)
		{
			throw std::invalid_argument("Bloomfilter could not be initialized: hash_func_count too large, hash_func_count *  bytes_per_hash_function must be smaller or equal to MD5_result_size_bytes");
		}
		bloomfilter_store_.assign(bloom_storage_size,false);
    }
    ~Bloomfilter(){
        delete[] MD5_hash_result_buffer;
    }

    void insert(const std::string& object)
    {
        hash(object);
        const u_int16_t* const object_hashes = reinterpret_cast<const u_int16_t * const>(MD5_hash_result_buffer);

        for (size_t i = 0; i < hash_function_count; i++)
		{
			const u_int16_t index_to_set = object_hashes[i] % bloomfilter_store_size;
			bloomfilter_store_[index_to_set] = true;
		}
		++object_count_;
    }

    void clear()
	{
		bloomfilter_store_.reserve(bloomfilter_store_size);
		bloomfilter_store_.assign(bloomfilter_store_size,false);
		object_count_ = 0;
	}

    bool contains(const std::string& object) const
	{
		hash(object);
		const u_int16_t* const object_hashes = reinterpret_cast<const u_int16_t * const>(MD5_hash_result_buffer);		

		for (size_t i = 0; i < hash_function_count; i++)
		{
			const u_int16_t index_to_get = object_hashes[i] % bloomfilter_store_size;
			if (!bloomfilter_store_[index_to_get]) return false;
		}
		return true;
	}

    size_t object_count() const
	{
		return object_count_;
	}

	bool empty() const
	{
		return 0 == object_count();
	}

};



#endif // !BLOOMFILTER_HPP
