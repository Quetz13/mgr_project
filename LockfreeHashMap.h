#pragma once

#include <atomic>
#include <cstdint>
#include <cmath>

template<typename T, std::uint32_t ElementSize>
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
			delete [] tmp->value;
			delete tmp;	
		}
	}

	unsigned int lastIndex() const
	{
		if (head.load() != nullptr)
			return head.load()->index + 1;
		else
			return 0;
	}

	// thread safe & lockfree
	bool TryToAdd(const std::uint32_t index)
	{
		std::uint32_t currentIndex = 0;
		do
		{
			Element* newBucket = nullptr;
			Element* tempHead = nullptr;
			do
			{
				if (newBucket)
				{
					delete [] newBucket->value;
					delete newBucket;
				}

				// pobranie obecnej g³owy
				tempHead = head;
				if (tempHead != nullptr)
				{
					currentIndex = tempHead->index + 1;
					// sprawdzenie czy nie ma ju¿ Bucketa o takim indeksie
					if (index <= tempHead->index)
						return false;
				}

				newBucket = new Element(new T[ElementSize], currentIndex);
				newBucket->next = tempHead;

			//} while (!std::atomic_compare_exchange_strong(&head, tempHead, newBucket));
			} while (!head.compare_exchange_strong(tempHead, newBucket));
		} while (currentIndex < index);

		return true;
	}

	// thread safe & lockfree
	T* Get(std::uint32_t index) const
	{
		Element* bucket = head;
		// sprawdzenie czy szukany bucket istnieje
		if (bucket == nullptr || index > bucket->index)
			return nullptr;

		// znalezienie odpowiedniego bucketa
		// TODO mo¿na zoptymalizowaæ zapewne, zmniejszyæ iloœæ sprawdzeñ
		while (bucket != nullptr && bucket->index != index)
			bucket = bucket->next;

		return bucket->value;
	}

};

//std::uint32_t(*Hash)(std::uint32_t);

template<typename KeyType, typename DataType, std::uint32_t BucketSize, std::uint32_t(*HashFunction)(KeyType)>
class LockFreeHashTable
{
private:
	struct Bucket
	{
		std::atomic<DataType*> value;
	};

	BucketsList<Bucket, BucketSize> _buckets;

	std::uint32_t getBucketIndex(std::uint32_t key) const
	{
		return (std::uint32_t)std::ceil(key / BucketSize);
	}

	std::uint32_t getKeyLocalIndex(std::uint32_t key) const
	{
		return key % BucketSize;
	}

public:
	void Set(KeyType key, DataType* value)
	{
		auto hashKey = HashFunction(key);
		auto bucketIndex = getBucketIndex(hashKey);

		// dodanie bucketów jeœli trzeba
		if (bucketIndex >= _buckets.lastIndex())
		{
			//auto newBucket = new Bucket[BucketSize];
			//if (!_buckets.TryToAdd(newBucket, bucketIndex))
				//delete newBucket;
			_buckets.TryToAdd(bucketIndex);
		}
		Bucket* bucket = _buckets.Get(bucketIndex);

		DataType* prev = bucket[getKeyLocalIndex(hashKey)].value.exchange(value);
		delete prev;
	}

	const DataType* Get(KeyType key) const
	{
		std::uint32_t bucketIndex = getBucketIndex(key);
		auto bucket = _buckets.Get(bucketIndex);
		if (bucket != nullptr)
			return _buckets.Get(bucketIndex)[key - bucketIndex*BucketSize].value.load();
		else 
			return nullptr;
	}

};