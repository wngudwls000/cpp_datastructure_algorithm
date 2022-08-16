#include <iostream>
#include <vector>

#include <openssl/md5.h>

class bloom_filter
{
	std::vector<bool> data;
	int hash_num;

	static constexpr int hashSize = 128 / 8; // 128��Ʈ (16����Ʈ)

	unsigned char hashValue[hashSize];

public:
	bloom_filter(int size, int hash_n) : hash_num(hash_n), data(size)
	{
		if (hash_n > hashSize)
		{
			throw("�ؽ� �Լ� ������ �ʹ� �����ϴ�.");
		}
		if (size > 255)
		{
			throw("��� ���� ũ�Ⱑ 255���� Ŭ �� �����ϴ�.");
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
				std::cout << email << " : ����� �� �ִ� �̸����Դϴ�." << std::endl;
				return false;
			}
		}
		std::cout << email << " : �̹� ��� ���Դϴ�." << std::endl;
		return true;
	}

	void insert(const std::string& email)
	{
		hash(email);
		for (auto it = &hashValue[0]; it < &hashValue[hash_num]; it++)
		{
			data[*it % data.size()] = true;
		}
		std::cout << "��� ���Ϳ� �߰�: " << email << std::endl;
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
