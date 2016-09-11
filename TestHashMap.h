#pragma once
#include <thread>
#include <vector>
#include "tbb\concurrent_hash_map.h"
#include "UnorderedMap.h"
#include "LockfreeHashMap.h"

namespace testHM
{
	std::uint32_t hashFun(std::uint32_t value)
	{
		return value;
	}

	typedef void (*SetFunction)(unsigned int);

	std::vector<std::thread*> _threads;
	std::atomic_bool _startFlag;
	
#pragma region HASH_TABLES
	tbb::concurrent_hash_map<std::uint32_t, std::uint32_t> tbbHashMap;
	UnorderedMap<std::uint32_t, std::uint32_t> stdMap;
	LockFreeHashTable < std::uint32_t, std::uint32_t, 1, hashFun> lockFreeTable1;
	LockFreeHashTable < std::uint32_t, std::uint32_t, 10, hashFun> lockFreeTable10;
	LockFreeHashTable < std::uint32_t, std::uint32_t, 100, hashFun> lockFreeTable100;
	LockFreeHashTable < std::uint32_t, std::uint32_t, 1000, hashFun> lockFreeTable1000;
	LockFreeHashTable < std::uint32_t, std::uint32_t, 10000, hashFun> lockFreeTable10000;
#pragma endregion

#pragma region SET_FUNCTIONS
	// SET
	void stdMap_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			stdMap.Insert(i, i);
	}

	void tbbHashMap_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			tbbHashMap.insert(std::make_pair(i, i));
	}

	void lockFreeTable1_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			lockFreeTable1.Set(i, new std::uint32_t(i));
	}

	void lockFreeTable10_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			lockFreeTable10.Set(i, new std::uint32_t(i));
	}

	void lockFreeTable100_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			lockFreeTable100.Set(i, new std::uint32_t(i));
	}

	void lockFreeTable1000_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			lockFreeTable1000.Set(i, new std::uint32_t(i));
	}

	void lockFreeTable10000_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			lockFreeTable10000.Set(i, new std::uint32_t(i));
	}
#pragma endregion

#pragma region GET_FUNCTIONS

#pragma endregion

#pragma region TEST_FUNCTIONS


	double TestThreads(SetFunction function, unsigned int threads, unsigned int elements)
	{
		using namespace std::chrono;
		time_point<high_resolution_clock> start;
		time_point<high_resolution_clock> stop;

		for (auto i = 0u; i < threads; i++)
			_threads[i] = new std::thread(function, elements);

		start = high_resolution_clock::now();
		_startFlag = true;

		for (auto t : _threads)
			t->join();
		for (auto t : _threads)
			delete t;

		stop = high_resolution_clock::now();
		_startFlag = false;

		return duration<double>(stop - start).count();
	}

	// TEST
	void testInsert(unsigned int threads, unsigned int elements)
	{
		using namespace std::chrono;
		time_point<high_resolution_clock> start;
		time_point<high_resolution_clock> stop;



		for (auto i = 0u; i < threads; i++)
			_threads.push_back(nullptr);

		// TBB
		for (auto i = 0u; i < threads; i++)
			_threads[i] = new std::thread(tbbHashMap_SetThread, elements);

		start = high_resolution_clock::now();
		_startFlag = true;

		for (auto t : _threads)
			t->join();
		for (auto t : _threads)
			delete t;

		stop = high_resolution_clock::now();
		auto tbbTime = duration<double>(stop - start).count();

		_startFlag = false;

		// UNORDERED
		for (auto i = 0u; i < threads; i++)
			_threads[i] = new std::thread(stdMap_SetThread, elements);

		start = high_resolution_clock::now();
		_startFlag = true;

		for (auto t : _threads)
			t->join();
		for (auto t : _threads)
			delete t;

		stop = high_resolution_clock::now();
		auto stdTime = duration<double>(stop - start).count();

		_startFlag = false;

		// LOCKFREE1
		for (auto i = 0u; i < threads; i++)
			_threads[i] = new std::thread(lockFreeTable1_SetThread, elements);

		start = high_resolution_clock::now();
		_startFlag = true;

		for (auto t : _threads)
			t->join();
		for (auto t : _threads)
			delete t;

		stop = high_resolution_clock::now();
		auto freetime1 = duration<double>(stop - start).count();

		_startFlag = false;

		// LOCKFREE10
		for (auto i = 0u; i < threads; i++)
			_threads[i] = new std::thread(lockFreeTable10_SetThread, elements);

		start = high_resolution_clock::now();
		_startFlag = true;

		for (auto t : _threads)
			t->join();
		for (auto t : _threads)
			delete t;

		stop = high_resolution_clock::now();
		auto freetime10 = duration<double>(stop - start).count();

		_startFlag = false;

		// LOCKFREE100
		for (auto i = 0u; i < threads; i++)
			_threads[i] = new std::thread(lockFreeTable100_SetThread, elements);

		start = high_resolution_clock::now();
		_startFlag = true;

		for (auto t : _threads)
			t->join();
		for (auto t : _threads)
			delete t;

		stop = high_resolution_clock::now();
		auto freetime100 = duration<double>(stop - start).count();

		_startFlag = false;

		// LOCKFREE1000
		for (auto i = 0u; i < threads; i++)
			_threads[i] = new std::thread(lockFreeTable1000_SetThread, elements);

		start = high_resolution_clock::now();
		_startFlag = true;

		for (auto t : _threads)
			t->join();
		for (auto t : _threads)
			delete t;

		stop = high_resolution_clock::now();
		auto freetime1000 = duration<double>(stop - start).count();

		_startFlag = false;

		// LOCKFREE10000
		for (auto i = 0u; i < threads; i++)
			_threads[i] = new std::thread(lockFreeTable10000_SetThread, elements);

		start = high_resolution_clock::now();
		_startFlag = true;

		for (auto t : _threads)
			t->join();
		for (auto t : _threads)
			delete t;

		stop = high_resolution_clock::now();
		auto freetime10000 = duration<double>(stop - start).count();

		_startFlag = false;

		std::cout << "Time TBB:\t\t" << tbbTime << " [" << tbbHashMap.size() << "]" << std::endl;
		std::cout << "Time STD:\t\t" << stdTime << " [" << stdMap.Size() << "]" << std::endl;
		std::cout << "Time LOCKFREE1:\t\t" << freetime1 << std::endl;
		std::cout << "Time LOCKFREE10:\t" << freetime10 << std::endl;
		std::cout << "Time LOCKFREE100:\t" << freetime100 << std::endl;
		std::cout << "Time LOCKFREE1000:\t" << freetime1000 << std::endl;
		std::cout << "Time LOCKFREE10000:\t" << freetime10000 << std::endl;
	}
#pragma endregion

}