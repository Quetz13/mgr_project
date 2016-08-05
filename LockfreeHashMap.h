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
			delete [] tmp->value;
			delete tmp;	
		}
	}

	unsigned int size() const
	{
		if (head.load() != nullptr)
			return head.load()->index + 1;
		else
			return 0;
	}

	// thread safe & lockfree
	bool TryToAdd(T* value, const std::uint32_t index)
	{
		std::uint32_t currentIndex = 0;
		do
		{
			Element* newBucket = nullptr;
			Element* tempHead = nullptr;
			do
			{
				if (newBucket)
					delete newBucket;
				// pobranie obecnej g³owy
				tempHead = head;
				if (tempHead != nullptr)
				{
					currentIndex = tempHead->index + 1;
					// sprawdzenie czy nie ma ju¿ Bucketa o takim indeksie
					if (index <= tempHead->index)
						return false;
				}

				newBucket = new Element(value, currentIndex);
				newBucket->next = tempHead;

			//} while (!std::atomic_compare_exchange_strong(&head, tempHead, newBucket));
			} while (!head.compare_exchange_strong(tempHead, newBucket));
		} while (currentIndex != index);

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

	std::uint32_t getBucketIndex(std::uint32_t key) const
	{
		return (std::uint32_t)std::ceil(key / BucketSize);
	}

	std::uint32_t getKeyLocalIndex(std::uint32_t key) const
	{
		return key % BucketSize;
	}

public:
	void Set(std::uint32_t key, T* value)
	{
		auto bucketIndex = getBucketIndex(key);

		// dodanie bucketów jeœli trzeba
		if (bucketIndex >= _buckets.size())
		{
			auto newBucket = new Bucket[BucketSize];
			if (!_buckets.TryToAdd(newBucket, bucketIndex))
				delete newBucket;
		}
		Bucket* bucket = _buckets.Get(bucketIndex);

		bucket[getKeyLocalIndex(key)].value.store(value);
	}

	const T* Get(std::uint32_t key) const
	{
		std::uint32_t bucketIndex = getBucketIndex(key);
		auto bucket = _buckets.Get(bucketIndex);
		if (bucket != nullptr)
			return _buckets.Get(bucketIndex)[key - bucketIndex*BucketSize].value.load();
		else 
			return nullptr;
	}

};