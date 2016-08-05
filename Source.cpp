#include "Test.h"
#include <chrono>
#include <string>
#include <iostream>
#include <thread>
#include <vector>

#include "HashTable.h"
#include "LockfreeHashMap.h"


HashTable<std::uint32_t, uint32_t_hash, ELEMENTS> uint32_table;

void HashSet(int num)
{
	int c = 0;
	int index = 1;
	for (uint32_t i = 1; i <= 5 * ELEMENTS; i++)
	{
		uint32_table.SetElement(index++, i);
		
		if (index == ELEMENTS + 1)
			index = 1;

		c++;
	}

	print("Producent " + to_string(num) + ": " + to_string(c));
}

void HashGet(int num)
{
	int c = 0;
	int index = 1;
	for (uint32_t i = 1; i <= 10 * ELEMENTS; i++)
	{
		auto x = uint32_table.GetElement(index++);

		if (index == ELEMENTS + 1)
			index = 1;

		c++;
	}

	print("Consument " + to_string(num) + ": " + to_string(c));
}

//void HashSetStr(int num)
//{
//	int c = 0;
//	int index = 1;
//	for (uint32_t i = 1; i <= 5 * ELEMENTS; i++)
//	{
//		uint32_table.SetElement(index++, i);
//
//		if (index == ELEMENTS + 1)
//			index = 1;
//
//		c++;
//	}
//
//	print("Producent " + to_string(num) + ": " + to_string(c));
//}
//
//void HashGetStr(int num)
//{
//	int c = 0;
//	int index = 1;
//	for (uint32_t i = 1; i <= 10 * ELEMENTS; i++)
//	{
//		auto x = uint32_table.GetElement(index++);
//
//		if (index == ELEMENTS + 1)
//			index = 1;
//
//		c++;
//	}
//
//	print("Consument " + to_string(num) + ": " + to_string(c));
//}

void testHASH(int producers, int consumers, void produce(int), void consume(int))
{
	using namespace std::chrono;

	time_point<high_resolution_clock> start = high_resolution_clock::now();

	std::vector<thread> consumersThreads;
	std::vector<thread> producersThreads;




	// tworzenie w¹tków producenta
	for (int i = 0; i < producers; i++)
		producersThreads.push_back(thread(produce, i));

	// tworzenie w¹tków konsumenta
	for (int i = 0; i < consumers; i++)
		consumersThreads.push_back(thread(consume, i));

	for (int i = 0; i < producers; i++)
		producersThreads[i].join();

	for (int i = 0; i <consumers; i++)
		consumersThreads[i].join();

	time_point<high_resolution_clock> stop = high_resolution_clock::now();

	std::cout << "Time: " << duration<double>(stop - start).count() << std::endl;

}


int main()
{
	//mainTests();
	//testHASH(PRODUCERS, CONSUMERS, HashSet, HashGet);

	LockFreeHashTable<int, 1024> ht;
	int* value = new int(13);
	ht.Set(10, value);
	auto result = ht.Get(10);

	if (result == nullptr)
		cout << "NULL" << endl;
	else
		cout << *result << endl;

	cin.get();
	return 0;
	//testCounterSpeed(5, 1000, 1000);
	
	
}