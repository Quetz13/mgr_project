#pragma once

#include <atomic>
#include <cstdint>
#include <cmath>

template<typename T>
class BucketsList
{
private:
	struct Element
	{
		Element(T* value, std::uint32_t index) : value(value), index(index), next(nullptr) {}
		T* value;
		std::atomic<Element*> next;
		const std::uint32_t index;
	};

	std::atomic<Element*> head;

public:

	// not thread safe
	BucketsList()
	{
		head = nullptr;
	}

	// not thread safe
	~BucketsList()
	{
		while (head.load() != nullptr)
		{
			Element* tmp = head.load();
			head.store(tmp->next);
			delete tmp->value;
			delete tmp;	
		}
	}

	// thread safe & lockfree
	// TODO kwestia indeksu, zeby w pêtli dodawaæ
	void TryToAdd(T* value, const std::uint32_t index)
	{
		Element* newBucket = nullptr;
		do
		{
			if (newBucket)
				delete newBucket;
			// pobranie obencje g³owy
			Element* tempHead = head;
			std::uint32_t newIndex = 0;
			if (tempHead != nullptr)
			{
				newIndex = tempHead->index + 1;
				// sprawdzenie czy nie ma ju¿ Bucketa o takim indeksie
				if (index <= tempHead->index)
					return;
			}

			newBucket = new Element(value, newIndex);
			newBucket->next = tempHead;

		} while (std::atomic_compare_exchange_strong(head, tempHead, newBucket));
	}

	// thread safe & lockfree
	T* Get(std::uint32_t index) const
	{
		Element* bucket = head;
		if (index > bucket->index)
			return nullptr;

		while (bucket != nullptr && bucket->index != index)
			bucket = bucket->next;

		return bucket->value;
	}

};


template<typename T, std::uint32_t BucketSize>
class LockFreeHashTable
{
private:
	struct Bucket
	{
		std::atomic_uint32_t key;
		std::atomic<T*> value;
	};

	BucketsList<Bucket> _buckets;

	std::uint32_t getBucketIndex(std::uint32_t key)
	{
		return std::ceil(key / BucketSize) - 1;
	}

public:
	void Insert(std::uint32_t key, T* value)
	{
		auto bucketIndex = getBucketIndex(key);
		_buckets.TryToAdd(new Bucket[BucketSize], bucketIndex);

		Bucket* bucket = _buckets.Get(bucketIndex);

	}

	T Get(const std::uint32_t key) const
	{
		std::uint32_t bucketIndex = getBucketIndex(key);
		return *(_buckets.Get(bucketIndex)[key - bucketIndex*BucketSize].value.load());
	}


};