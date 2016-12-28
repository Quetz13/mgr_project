#pragma once
#include "tbb\concurrent_hash_map.h"

#include "UnorderedMap.h"
#include "LockfreeHashMap.h"


namespace hmtest
{
	std::uint32_t hashFun(std::uint32_t value)
	{
		return value;
	}


	template<typename KeyType, typename DataType>
	struct samples
	{
		typedef void(samples::*SetFun)(KeyType, DataType);
		typedef DataType(samples::*GetFun)(KeyType) const;

		samples(unsigned int threads, unsigned int elementsToWrite, unsigned int elementsToRead)
		{
			_startFlag.store(false);
			SetTest(threads, elementsToWrite);
		}

		void Join()
		{
			for (auto t : _threads)
				if (t)
					t->join();
			for (auto t : _threads)
			{
				delete t;
				t = nullptr;
			}
		}

		void Clear()
		{
			Join();
			_threads.clear();
		}

		void SetTest(unsigned int threads, unsigned int elementsToWrite)
		{
			using namespace std::chrono;

			time_point<high_resolution_clock> start;// = high_resolution_clock::now();
			time_point<high_resolution_clock> stop;// = high_resolution_clock::now();
			//std::cout << "Time: " << duration<double>(stop - start).count() << std::endl;
	
			for (auto i = 0u; i < threads; i++)
				_threads.push_back(nullptr);

			// TBB
			//for (auto i = 0u; i < threads; i++)
			//	SetThreadRun(_threads[i], &samples::insertTbb, elementsToWrite);
			//
			//start = high_resolution_clock::now();
			//_startFlag = true;

			//Join();

			//stop = high_resolution_clock::now();
			//auto tbbTime =  duration<double>(stop - start).count();

			_startFlag = false;

			// UNORDERED
			for (auto i = 0u; i < threads; i++)
				SetThreadRun(_threads[i], &samples::insertStdMap, elementsToWrite);

			start = high_resolution_clock::now();
			_startFlag = true;

			Join();

			stop = high_resolution_clock::now();
			auto stdTime = duration<double>(stop - start).count();

			_startFlag = false;

			//std::cout << "Time: " << tbbTime << " [" << tbbHashMap.size() << "]" << std::endl;
			std::cout << "Time: " << stdTime << " [" << stdMap.Size() << "]" << std::endl;
		}

		tbb::concurrent_hash_map<KeyType, DataType> tbbHashMap;
		UnorderedMap<KeyType, DataType> stdMap;
		LockFreeHashTable < KeyType, DataType, 1, hashFun> lockFreeTable1;
		LockFreeHashTable < KeyType, DataType, 10, hashFun> lockFreeTable10;
		LockFreeHashTable < KeyType, DataType, 100, hashFun> lockFreeTable100;
		LockFreeHashTable < KeyType, DataType, 1000, hashFun> lockFreeTable1000;
		LockFreeHashTable < KeyType, DataType, 10000, hashFun> lockFreeTable10000;
		
	private:
		std::vector<std::thread*> _threads;
		std::atomic_bool _startFlag;

		// TBB
		inline void insertTbb(KeyType key, DataType data)
		{
			tbbHashMap.insert(key, data);
		}

		inline DataType getTbb(KeyType key)
		{
			tbb::concurrent_hash_map<KeyType, DataType>::result result;
			tbbHashMap.find(result, key);
			return result;
		}

		// stdMap
		void insertStdMap(KeyType key, DataType data)
		{
			stdMap.Insert(key, data);
		}

		DataType getStdMap(KeyType key)
		{
			return stdMap.Get(key);
		}

		// stdMap
		void insertLockFreeTable1(KeyType key, DataType data)
		{
			lockFreeTable1.Set(key, data);
		}

		DataType getLockFreeTable1(KeyType key)
		{
			return lockFreeTable1.Get(key);
		}




		// THREADS FUN
		void SetThread(SetFun setFun, unsigned int elementsToWrite)
		{
			while (!_startFlag)
				std::this_thread::yield();

			for (auto i = 0u; i < elementsToWrite; i++)
			{
				//setFun((KeyType)i, (DataType)i);
				insertStdMap((KeyType)i, (DataType)i);
			}
		}

		void ReadThread(GetFun getFun, unsigned int elementsToRead)
		{
			while (!_startFlag)
				std::this_thread::yield();
			
			DataType data;
			for (auto i = 0; i < elementsToRead; i++)
			{
				data = GetFun((KeyType)i);
			}
		}

		void SetThreadRun(std::thread*t, SetFun setFun, unsigned int elementsToWrite)
		{
			t = new thread(&samples::SetThread, this, setFun, elementsToWrite);
		}

		void ReadThreadRun(std::thread*t, GetFun getFun, unsigned int elementsToRead)
		{
			t = new thread(&samples::ReadThread, this, getFun, elementsToRead);
		}
		
		
	
	};

	
	

	





}