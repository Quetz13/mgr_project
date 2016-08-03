#pragma once
#include <atomic>
#include <cstdint>

class LockFreeArray
{
public:

	LockFreeArray(uint32_t)
	{
		nElements = new Element[10];
	}

	struct Element
	{
		std::atomic_uint32_t key;
		std::atomic_uint32_t value;
	};

	Element* nElements;

	void SetElement(std::uint32_t key, std::uint32_t value)
	{
		for (uint32_t idx = 0;; idx++)
		{
			uint32_t prevKey = std::atomic_compare_exchange_strong_explicit(&nElements[idx].key, 0, key, std::memory_order::memory_order_relaxed, std::memory_order::memory_order_relaxed);

			if ((prevKey == 0) || (prevKey == key))
			{
				std::atomic_store_explicit(&nElements[idx].value, value, std::memory_order::memory_order_relaxed);
				return;
			}
		}
	}

	void SetElementFast(std::uint32_t key, std::uint32_t value)
	{
		for (uint32_t idx = 0;; idx++)
		{
			// Load the key that was there.
			uint32_t probedKey = std::atomic_load_explicit(&nElements[idx].key, std::memory_order::memory_order_relaxed);
			if (probedKey != key)
			{
				// The entry was either free, or contains another key.
				if (probedKey != 0)
					continue;           // Usually, it contains another key. Keep probing.

				// The entry was free. Now let's try to take it using a CAS.
				uint32_t prevKey = std::atomic_compare_exchange_strong_explicit(&nElements[idx].key, 0, key, std::memory_order::memory_order_relaxed, std::memory_order::memory_order_relaxed);
				if ((prevKey != 0) && (prevKey != key))
					continue;       // Another thread just stole it from underneath us.

				// Either we just added the key, or another thread did.
			}

			// Store the value in this array entry.
			std::atomic_store_explicit(&nElements[idx].value, value, std::memory_order::memory_order_relaxed);
			return;
		}
	}

	uint32_t GetElement(std::uint32_t key)
	{
		for (uint32_t idx = 0;; idx++)
		{
			uint32_t resultKey = std::atomic_load_explicit(&nElements[idx].key,std::memory_order::memory_order_relaxed);

			if (resultKey == key)
				return std::atomic_load_explicit(&nElements[idx].value, std::memory_order::memory_order_relaxed);
			else
				return 0;
		}
	}

};


inline std::uint32_t uint32_t_hash(uint32_t h)
{
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h;
}

template<typename T, std::uint32_t(*HashFun)(T)>
class HashTable
{
public:

	HashTable(uint32_t size) : nSize(size)
	{
		//nElements = new Element[nSize];
	}

	struct Element
	{
		std::atomic_uint32_t key;
		std::atomic<T> value;
	};

	Element nElements[20];


	void SetElement(std::uint32_t key, T value)
	{
		

		for (uint32_t idx = HashFun(key);; idx++)
		{
			idx &= nSize - 1;

			uint32_t expected = 0;					
			uint32_t prevKey = std::atomic_compare_exchange_strong(&nElements[idx].key, &expected, key);
			
			//uint32_t prevKey = nElements[idx].key.compare_exchange_strong(expected, key);
			
			if ((prevKey == 0) || (prevKey == key))
			{
				std::atomic_store_explicit(&nElements[idx].value, value, std::memory_order::memory_order_relaxed);
				return;
			}
		}
	}



	T GetElement(std::uint32_t key)
	{
		for (uint32_t idx = HashFun(key);; idx++)
		{
			idx &= nSize - 1;

			uint32_t resultKey = std::atomic_load_explicit(&nElements[idx].key, std::memory_order::memory_order_relaxed);
			if (resultKey == key)
				return std::atomic_load_explicit(&nElements[idx].value, std::memory_order::memory_order_relaxed);
			else
				return 0;
		}
	}

private:
	std::uint32_t nSize;
};

