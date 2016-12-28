#pragma once
//#define JUNCTION
#include <thread>
#include <vector>
#ifdef TBB
#include "tbb\concurrent_hash_map.h"
#endif
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
#ifdef TBB
	tbb::concurrent_hash_map<std::uint32_t, std::uint32_t> tbbHashMap;
#endif	
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

	void printSizes()
	{
		//cout << endl;
		//cout << "STD size: " << sizeof(stdMap) << endl;
		//cout << "lockFreeTable1 size: " << sizeof(lockFreeTable1) << endl;
		//cout << "lockFreeTable10 size: " << sizeof(lockFreeTable10) << endl;
		//cout << "lockFreeTable100 size: " << sizeof(lockFreeTable100) << endl;
		//cout << "lockFreeTable1000 size: " << sizeof(lockFreeTable1000) << endl;
		//cout << "lockFreeTable10000 size: " << sizeof(lockFreeTable10000) << endl;
		//cout << endl;
	}

#pragma region SET_FUNCTIONS
	// SET
	void stdMap_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			stdMap.Insert(i, i);
	}

#ifdef TBB
	void tbbHashMap_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			tbbHashMap.insert(std::make_pair(i, i));
	}
#endif
	void lockFreeTable1_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			lockFreeTable1.Set(i, new std::uint32_t(i));
			//lockFreeTable1.Set(i, i);
	}
#ifdef JUNCTION
	void junctionLinear_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			junctionLinear.assign(i,i);
	}

	void junctionCrude_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			junctionCrude.assign(i, i);
	}

	void junctionGrampa_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			junctionGrampa.assign(i, i);
	}

	void junctionLeapFrog_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			junctionLeapfrog.assign(i, i);
	}
#endif
	void lockFreeTable10_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			lockFreeTable10.Set(i, new std::uint32_t(i));
			//lockFreeTable10.Set(i, i);
	}

	void lockFreeTable100_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			lockFreeTable100.Set(i, new std::uint32_t(i));
			//lockFreeTable100.Set(i, i);
	}

	void lockFreeTable1000_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			lockFreeTable1000.Set(i, new std::uint32_t(i));
			//lockFreeTable1000.Set(i, i);
	}

	void lockFreeTable10000_SetThread(unsigned int elementsToWrite)
	{
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			lockFreeTable10000.Set(i, new std::uint32_t(i));
			//lockFreeTable10000.Set(i, i);
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
#ifdef TBB
	void tbbHashMap_GetThread(unsigned int elementsToWrite)
	{
		tbb::concurrent_hash_map<std::uint32_t, std::uint32_t>::accessor result;
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			tbbHashMap.find(result, i);
	}
#endif
#ifdef JUNCTION
	void junctionLinear_GetThread(unsigned int elementsToWrite)
	{
		tbb::concurrent_hash_map<std::uint32_t, std::uint32_t>::accessor result;
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			auto value = junctionLinear.get(i);
	}

	void junctionGrampa_GetThread(unsigned int elementsToWrite)
	{
		tbb::concurrent_hash_map<std::uint32_t, std::uint32_t>::accessor result;
		while (!_startFlag)
			std::this_thread::yield();

		for (std::uint32_t i = 0u; i < elementsToWrite; i++)
			auto value = junctionGrampa.get(i);
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
			auto value = junctionLeapfrog.get(i);
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
#ifdef TBB
		auto tbbTime = TestThreads(tbbHashMap_SetThread, threads, elements);
		std::cout << "Time TBB:\t\t" << tbbTime << " [" << tbbHashMap.size() << "]" << std::endl;
#endif
		// UNORDERED
		auto stdTime = TestThreads(stdMap_SetThread, threads, elements);
		std::cout << "Time STD:\t\t" << stdTime << " [" << stdMap.Size() << "]" << std::endl;
#ifdef JUNCTION
		// junctionLinear
		//auto linearTime = TestThreads(junctionLinear_SetThread, threads, elements);
		//std::cout << "Time JunctionLinear:\t\t" << linearTime << std::endl;
		// junctionGrampa
		//auto grampaTime = TestThreads(junctionGrampa_SetThread, threads, elements);
		//std::cout << "Time JunctionGrampa:\t" << grampaTime << std::endl;
		// junctionCrude
		auto crudeTime = TestThreads(junctionCrude_SetThread, threads, elements);
		std::cout << "Time JunctionCrude:\t" << crudeTime << std::endl;
		// junctionLeapFrog
		//auto leapFrogTime = TestThreads(junctionLeapFrog_SetThread, threads, elements);
		//std::cout << "Time JunctionLeapFrog:\t" << leapFrogTime << std::endl;
#endif
		// LOCKFREE1
		auto freetime1 = TestThreads(lockFreeTable1_SetThread, threads, elements);
		std::cout << "Time LOCKFREE1:\t\t" << freetime1 << std::endl;
		// LOCKFREE10
		auto freetime10 = TestThreads(lockFreeTable10_SetThread, threads, elements);
		std::cout << "Time LOCKFREE10:\t" << freetime10 << std::endl;
		// LOCKFREE100
		auto freetime100 = TestThreads(lockFreeTable100_SetThread, threads, elements);
		std::cout << "Time LOCKFREE100:\t" << freetime100 << std::endl;
		// LOCKFREE1000
		auto freetime1000 = TestThreads(lockFreeTable1000_SetThread, threads, elements);
		std::cout << "Time LOCKFREE1000:\t" << freetime1000 << std::endl;
		// LOCKFREE10000
		auto freetime10000 = TestThreads(lockFreeTable10000_SetThread, threads, elements);
		std::cout << "Time LOCKFREE10000:\t" << freetime10000 << std::endl;		
		cout << endl;
	}

	void testGet(unsigned int threads, unsigned int elements)
	{
#ifdef TBB
		// TBB
		auto tbbTime = TestThreads(tbbHashMap_GetThread, threads, elements);
		std::cout << "Time TBB:\t\t" << tbbTime << " [" << tbbHashMap.size() << "]" << std::endl;
#endif
		// UNORDERED
		auto stdTime = TestThreads(stdMap_GetThread, threads, elements);
		std::cout << "Time STD:\t\t" << stdTime << " [" << stdMap.Size() << "]" << std::endl;
#ifdef JUNCTION
		// junctionLinear
		//auto linearTime = TestThreads(junctionLinear_GetThread, threads, elements);
		//std::cout << "Time JunctionLinear:\t" << linearTime << std::endl;
		// junctionGrampa
		//auto grampaTime = TestThreads(junctionGrampa_GetThread, threads, elements);
		//std::cout << "Time JunctionGrampa:\t" << grampaTime << std::endl;
		// junctionCrude
		auto crudeTime = TestThreads(junctionCrude_GetThread, threads, elements);
		std::cout << "Time JunctionCrude:\t" << crudeTime << std::endl;
		// junctionLeapFrog
		//auto leapFrogTime = TestThreads(junctionLeapFrog_GetThread, threads, elements);
		//std::cout << "Time JunctionLeapFrog:\t" << leapFrogTime << std::endl;
#endif
		// LOCKFREE1
		auto freetime1 = TestThreads(lockFreeTable1_GetThread, threads, elements);
		std::cout << "Time LOCKFREE1:\t\t" << freetime1 << std::endl;
		// LOCKFREE10
		auto freetime10 = TestThreads(lockFreeTable10_GetThread, threads, elements);
		std::cout << "Time LOCKFREE10:\t" << freetime10 << std::endl;
		// LOCKFREE100
		auto freetime100 = TestThreads(lockFreeTable100_GetThread, threads, elements);
		std::cout << "Time LOCKFREE100:\t" << freetime100 << std::endl;
		// LOCKFREE1000
		auto freetime1000 = TestThreads(lockFreeTable1000_GetThread, threads, elements);
		std::cout << "Time LOCKFREE1000:\t" << freetime1000 << std::endl;
		// LOCKFREE10000
		auto freetime10000 = TestThreads(lockFreeTable10000_GetThread, threads, elements);
		std::cout << "Time LOCKFREE10000:\t" << freetime10000 << std::endl;
		std::cout << std::endl;
	}

	void testAll(unsigned int threadsSet, unsigned int threadsRead, unsigned int elements)
	{
		// TBB
#ifdef TBB
		auto tbbTime = TestThreads(tbbHashMap_SetThread, threadsSet, tbbHashMap_GetThread, threadsRead, elements);
		std::cout << "Time TBB:\t\t" << tbbTime << " [" << tbbHashMap.size() << "]" << std::endl;
#endif
		// UNORDERED
		auto stdTime = TestThreads(stdMap_SetThread, threadsSet, stdMap_GetThread, threadsRead, elements);
		std::cout << "Time STD:\t\t" << stdTime << " [" << stdMap.Size() << "]" << std::endl;
#ifdef JUNCTION
		// LINEAR
		//auto linearTime = TestThreads(junctionLinear_SetThread, threadsSet, junctionLinear_GetThread, threadsRead, elements);
		//std::cout << "Time JunctionLinear:\t" << linearTime << std::endl;
		// Crude
		auto crudeTime = TestThreads(junctionCrude_SetThread, threadsSet, junctionCrude_GetThread, threadsRead, elements);
		std::cout << "Time JunctionCrude:\t" << crudeTime << std::endl;
		// Grampa
		//auto grampaTime = TestThreads(junctionGrampa_SetThread, threadsSet, junctionGrampa_GetThread, threadsRead, elements);
		//std::cout << "Time JunctionGrampa:\t" << grampaTime << std::endl;
		// LeapFrog
		//auto leapFrogTime = TestThreads(junctionLeapFrog_SetThread, threadsSet, junctionLeapFrog_GetThread, threadsRead, elements);
		//std::cout << "Time JunctionLeapFrog:\t" << leapFrogTime << std::endl;
#endif
		// LOCKFREE1
		auto freetime1 = TestThreads(lockFreeTable1_SetThread, threadsSet, lockFreeTable1_GetThread, threadsRead, elements);
		std::cout << "Time LOCKFREE1:\t\t" << freetime1 << std::endl;
		// LOCKFREE10
		auto freetime10 = TestThreads(lockFreeTable10_SetThread, threadsSet, lockFreeTable10_GetThread, threadsRead, elements);
		std::cout << "Time LOCKFREE10:\t" << freetime10 << std::endl;
		// LOCKFREE100
		auto freetime100 = TestThreads(lockFreeTable100_SetThread, threadsSet, lockFreeTable100_GetThread, threadsRead, elements);
		std::cout << "Time LOCKFREE100:\t" << freetime100 << std::endl;
		// LOCKFREE1000
		auto freetime1000 = TestThreads(lockFreeTable1000_SetThread, threadsSet, lockFreeTable1000_GetThread, threadsRead, elements);
		std::cout << "Time LOCKFREE1000:\t" << freetime1000 << std::endl;
		// LOCKFREE10000
		auto freetime10000 = TestThreads(lockFreeTable10000_SetThread, threadsSet, lockFreeTable10000_GetThread, threadsRead, elements);
		std::cout << "Time LOCKFREE10000:\t" << freetime10000 << std::endl;
		std::cout << std::endl;
	}
#pragma endregion

}