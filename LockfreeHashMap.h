#pragma once

#include <atomic>
#include <cstdint>

template<typename T>
class BucketsList
{
private:
	struct Bucket
	{
		Bucket(T value, std::uint32_t index) : value(value), index(index), next(nullptr) {}
		T value;
		std::atomic<Bucket*> next;
		std::uint32_t index;
	};

	std::atomic<Bucket*> head;

public:

	// not thread safe
	BucketsList()
	{
		head = nullptr;
	}

	// not thread safe
	~BucketsList()
	{
		while (head != nullptr)
		{
			Bucket* tmp = head.load();
			head = tmp->next;
			delete tmp;
		}
	}

	// thread safe & lockfree
	void TryToAddBucket(T value, std::uint32_t index)
	{
		do
		{
			// pobranie obencje g³owy
			Bucket* tempHead = head;
			// sprawdzenie czy nie ma ju¿ Bucketu o takim indeksie
			if (index <= tempHead->index)
				return;
			
			Bucket* newBucket = new Bucket(value, index);
			newBucket->next = head.load();

		} while (std::atomic_compare_exchange_strong(head, tempHead, newBucket));
	}

	Bucket* GetBucket(std::uint32_t index)
	{
		Bucket* bucket = head;
		if (index > bucket->index)
			return nullptr;

		while (bucket != nullptr && bucket->index != index)
			bucket = bucket->next;

		return bucket;
	}

};