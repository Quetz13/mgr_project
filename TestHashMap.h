#pragma once
#include <thread>
#include <vector>
#include <assert.h>
#include "tbb\concurrent_hash_map.h"
#ifdef JUNCTION
#include "ConcurrentMap_Linear.h"
#include "ConcurrentMap_Grampa.h"
#include "ConcurrentMap_Leapfrog.h"
#include "ConcurrentMap_Crude.h"
#endif
#include "UnorderedMap.h"
#include "LockfreeHashMap.h"


namespace testHM
{
	std::uint32_t hashFun(std::uint32_t value)
	{
		return value;
	}

	typedef void (*Function)(unsigned int);

	std::atomic_bool _startFlag;
	
#pragma region HASH_TABLES
	tbb::concurrent_hash_map<std::uint32_t, std::uint32_t> tbbHashMap;
	UnorderedMap<std::uint32_t, std::uint32_t> stdMap;
#ifdef JUNCTION
	junction::ConcurrentMap_Linear<std::uint32_t, std::uint32_t> junctionLinear;
	junction::ConcurrentMap_Crude<std::uint32_t, std::uint32_t> junctionCrude;
	junction::ConcurrentMap_Grampa<std::uint32_t, std::uint32_t> junctionGrampa;
	junction::ConcurrentMap_Leapfrog<std::uint32_t, std::uint32_t> junctionLeapfrog;
#endif
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
			//lockFreeTable1.Set(i, new std::uint32_t(i));
			lockFreeTable1.Set(i, i);
	}
#ifdef JUNCTION
	//void junctionLinear_SetThread(unsigned int elementsToWrite)
	//{
	//	while (!_startFlag)
	//		std::this_thread::yield();

	//	for (std::uint32_t i = 0u; i < elementsToWrite; i++)
	//		junctionLinear.insertOrFind(i,i);
	//}
#endif
	void lockFreeTable10_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			//lockFreeTable10.Set(i, new std::uint32_t(i));
			lockFreeTable10.Set(i, i);
	}

	void lockFreeTable100_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			//lockFreeTable100.Set(i, new std::uint32_t(i));
			lockFreeTable100.Set(i, i);
	}

	void lockFreeTable1000_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			//lockFreeTable1000.Set(i, new std::uint32_t(i));
			lockFreeTable1000.Set(i, i);
	}

	void lockFreeTable10000_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			//lockFreeTable10000.Set(i, new std::uint32_t(i));
			lockFreeTable10000.Set(i, i);
	}
#pragma endregion

#pragma region GET_FUNCTIONS
	// GET
	void stdMap_GetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			auto value = stdMap.Get(i);
	}

	void tbbHashMap_GetThread(unsigned int elementsToWrite)
	{
		tbb::concurrent_hash_map<std::uint32_t, std::uint32_t>::accessor result;
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			tbbHashMap.find(result, i);
	}
#ifdef JUNCTION
	void junctionLinear_GetThread(unsigned int elementsToWrite)
	{
		tbb::concurrent_hash_map<std::uint32_t, std::uint32_t>::accessor result;
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			auto value = junctionLinear.find(i);
	}

	void junctionGrampa_GetThread(unsigned int elementsToWrite)
	{
		tbb::concurrent_hash_map<std::uint32_t, std::uint32_t>::accessor result;
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			auto value = junctionGrampa.find(i);
	}

	void junctionCrude_GetThread(unsigned int elementsToWrite)
	{
		tbb::concurrent_hash_map<std::uint32_t, std::uint32_t>::accessor result;
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			auto value = junctionCrude.get(i);
	}

	void junctionLeapFrog_GetThread(unsigned int elementsToWrite)
	{
		tbb::concurrent_hash_map<std::uint32_t, std::uint32_t>::accessor result;
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			auto value = junctionLeapfrog.find(i);
	}
#endif
	void lockFreeTable1_GetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			auto value = lockFreeTable1.Get(i);
	}

	void lockFreeTable10_GetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			auto value = lockFreeTable10.Get(i);
	}

	void lockFreeTable100_GetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			auto value = lockFreeTable100.Get(i);
	}

	void lockFreeTable1000_GetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			auto value = lockFreeTable1000.Get(i);
	}

	void lockFreeTable10000_GetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			auto value = lockFreeTable10000.Get(i);
	}
#pragma endregion

#pragma region TEST_FUNCTIONS


	double TestThreads(Function function, unsigned int threads, unsigned int elements)
	{
		using namespace std::chrono;
		std::vector<std::thread*> threadsVec;
		time_point<high_resolution_clock> start;
		time_point<high_resolution_clock> stop;

		for (auto i = 0u; i < threads; i++)
			threadsVec.push_back(new std::thread(function, elements));

		start = high_resolution_clock::now();
		_startFlag = true;

		for (auto t : threadsVec)
			t->join();
		stop = high_resolution_clock::now();

		for (auto t : threadsVec)
			delete t;
		_startFlag = false;

		return duration<double>(stop - start).count();
	}

	double TestThreads(Function function1, unsigned int threads1, Function function2, unsigned int threads2, unsigned int elements)
	{
		using namespace std::chrono;
		std::vector<std::thread*> threadsVec;
		time_point<high_resolution_clock> start;
		time_point<high_resolution_clock> stop;

		for (auto i = 0u; i < threads1; i++)
			threadsVec.push_back(new std::thread(function1, elements));
		for (auto i = 0u; i < threads2; i++)
			threadsVec.push_back(new std::thread(function2, elements));

		start = high_resolution_clock::now();
		_startFlag = true;

		for (auto t : threadsVec)
			t->join();
		stop = high_resolution_clock::now();

		for (auto t : threadsVec)
			delete t;
		_startFlag = false;

		return duration<double>(stop - start).count();
	}


	// TEST
	void testInsert(unsigned int threads, unsigned int elements)
	{
		// TBB
		auto tbbTime = TestThreads(tbbHashMap_SetThread, threads, elements);

		// UNORDERED
		auto stdTime = TestThreads(stdMap_SetThread, threads, elements);

		// LOCKFREE1
		auto freetime1 = TestThreads(lockFreeTable1_SetThread, threads, elements);

		// LOCKFREE10
		auto freetime10 = TestThreads(lockFreeTable10_SetThread, threads, elements);

		// LOCKFREE100
		auto freetime100 = TestThreads(lockFreeTable100_SetThread, threads, elements);

		// LOCKFREE1000
		auto freetime1000 = TestThreads(lockFreeTable1000_SetThread, threads, elements);

		// LOCKFREE10000
		auto freetime10000 = TestThreads(lockFreeTable10000_SetThread, threads, elements);

		std::cout << "Time TBB:\t\t" << tbbTime << " [" << tbbHashMap.size() << "]" << std::endl;
		std::cout << "Time STD:\t\t" << stdTime << " [" << stdMap.Size() << "]" << std::endl;
		std::cout << "Time LOCKFREE1:\t\t" << freetime1 << std::endl;
		std::cout << "Time LOCKFREE10:\t" << freetime10 << std::endl;
		std::cout << "Time LOCKFREE100:\t" << freetime100 << std::endl;
		std::cout << "Time LOCKFREE1000:\t" << freetime1000 << std::endl;
		std::cout << "Time LOCKFREE10000:\t" << freetime10000 << std::endl;
		cout << endl;
	}

	void testGet(unsigned int threads, unsigned int elements)
	{
		// TBB
		auto tbbTime = TestThreads(tbbHashMap_GetThread, threads, elements);

		// UNORDERED
		auto stdTime = TestThreads(stdMap_GetThread, threads, elements);
#ifdef JUNCTION
		// junctionLinear
		auto linearTime = TestThreads(junctionLinear_GetThread, threads, elements);

		// junctionGrampa
		auto grampaTime = TestThreads(junctionGrampa_GetThread, threads, elements);

		// junctionCrude
		auto crudeTime = TestThreads(junctionCrude_GetThread, threads, elements);

		// junctionLeapFrog
		auto leapFrogTime = TestThreads(junctionLeapFrog_GetThread, threads, elements);
#endif
		// LOCKFREE1
		auto freetime1 = TestThreads(lockFreeTable1_GetThread, threads, elements);

		// LOCKFREE10
		auto freetime10 = TestThreads(lockFreeTable10_GetThread, threads, elements);

		// LOCKFREE100
		auto freetime100 = TestThreads(lockFreeTable100_GetThread, threads, elements);

		// LOCKFREE1000
		auto freetime1000 = TestThreads(lockFreeTable1000_GetThread, threads, elements);

		// LOCKFREE10000
		auto freetime10000 = TestThreads(lockFreeTable10000_GetThread, threads, elements);

		std::cout << "Time TBB:\t\t" << tbbTime << " [" << tbbHashMap.size() << "]" << std::endl;
		std::cout << "Time STD:\t\t" << stdTime << " [" << stdMap.Size() << "]" << std::endl;
#ifdef JUNCTION
		std::cout << "Time JunctionLinear:\t\t" << linearTime << std::endl;
		std::cout << "Time JunctionCrude:\t\t" << crudeTime << std::endl;
		std::cout << "Time JunctionGrampa:\t\t" << grampaTime << std::endl;
		std::cout << "Time JunctionLeapFrog:\t\t" << leapFrogTime << std::endl;
#endif
		std::cout << "Time LOCKFREE1:\t\t" << freetime1 << std::endl;
		std::cout << "Time LOCKFREE10:\t" << freetime10 << std::endl;
		std::cout << "Time LOCKFREE100:\t" << freetime100 << std::endl;
		std::cout << "Time LOCKFREE1000:\t" << freetime1000 << std::endl;
		std::cout << "Time LOCKFREE10000:\t" << freetime10000 << std::endl;
	}

	void testAll(unsigned int threadsSet, unsigned int threadsRead, unsigned int elements)
	{
		// TBB
		auto tbbTime = TestThreads(tbbHashMap_SetThread, threadsSet, tbbHashMap_GetThread, threadsRead, elements);

		// UNORDERED
		auto stdTime = TestThreads(stdMap_SetThread, threadsSet, stdMap_GetThread, threadsRead, elements);

		// LOCKFREE1
		auto freetime1 = TestThreads(lockFreeTable1_SetThread, threadsSet, lockFreeTable1_GetThread, threadsRead, elements);

		// LOCKFREE10
		auto freetime10 = TestThreads(lockFreeTable10_SetThread, threadsSet, lockFreeTable10_GetThread, threadsRead, elements);

		// LOCKFREE100
		auto freetime100 = TestThreads(lockFreeTable100_SetThread, threadsSet, lockFreeTable100_GetThread, threadsRead, elements);

		// LOCKFREE1000
		auto freetime1000 = TestThreads(lockFreeTable1000_SetThread, threadsSet, lockFreeTable1000_GetThread, threadsRead, elements);

		// LOCKFREE10000
		auto freetime10000 = TestThreads(lockFreeTable10000_SetThread, threadsSet, lockFreeTable10000_GetThread, threadsRead, elements);



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