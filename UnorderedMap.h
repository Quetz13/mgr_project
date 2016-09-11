#pragma once
#include <unordered_map>
#include <mutex>

template<typename KeyType, typename DataType>
class UnorderedMap : private std::unordered_map <KeyType, DataType>
{
public:
	UnorderedMap() = default;
	~UnorderedMap() = default;

	void Insert(KeyType key, DataType data)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		this->insert(std::make_pair(key, data));
	}

	DataType Get(const KeyType &key)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return (*this)[key];
	}

	size_t Size()
	{
		return size();
	}

private:
	std::mutex _mutex;

};