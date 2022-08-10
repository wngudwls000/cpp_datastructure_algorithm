#include <iostream>
#include <vector>

#include <openssl/md5.h>

class bloom_filter
{
	std::vector<bool> data;
	int hash_num;

	static constexpr int hashSize = 128 / 8; // 128비트 (16바이트)

	unsigned char hashValue[hashSize];

public:
	bloom_filter(int size, int hash_n) : hash_num(hash_n), data(size)
	{
		if (hash_n > hashSize)
		{
			throw("해시 함수 개수가 너무 많습니다.");
		}
		if (size > 255)
		{
			throw("블룸 필터 크기가 255보다 클 수 없습니다.");
		}
	}

	void hash(const std::string& key)
	{
		MD5(reinterpret_cast<const unsigned char*>(key.data()), key.length(), hashValue);
	}

	bool lookup(const std::string& email)
	{
		hash(email);
		for (auto it = &hashValue[0]; it < &hashValue[hash_num]; it++)
		{
			if (!data[*it % data.size()])
			{
				std::cout << email << " : 사용할 수 있는 이메일입니다." << std::endl;
				return false;
			}
		}
		std::cout << email << " : 이미 사용 중입니다." << std::endl;
		return true;
	}

	void insert(const std::string& email)
	{
		hash(email);
		for (auto it = &hashValue[0]; it < &hashValue[hash_num]; it++)
		{
			data[*it % data.size()] = true;
		}
		std::cout << "블룸 필터에 추가: " << email << std::endl;
	}
};

int main()
{
	bloom_filter bf(128, 5);

	bf.insert("abc@gilbut.com");
	bf.insert("xyz@gilbut.com");

	bf.lookup("abc");
	bf.lookup("xyz@gilbut.com");
	bf.lookup("xyz");

	bf.insert("abcd@gilbut.com");
	bf.insert("ab@gilbut.com");

	bf.lookup("abcd");
	bf.lookup("ab@gilbut.com");
}
