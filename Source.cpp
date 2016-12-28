#include "Test.h"
#include <chrono>
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <type_traits>
#include "MemoryOrder.h"

//#include "HashTable.h"
#include "LockfreeHashMap.h"

#include "TestHashMap.h"

//std::uint32_t hashFun(std::uint32_t value)
//{
//	return value;
//}
//
//LockFreeHashTable<std::uint32_t, std::uint32_t, ELEMENTS, hashFun> uint32_table;
//
//void HashSet(int num)
//{
//	int c = 0;
//	int index = 1;
//	for (uint32_t i = 1; i <= 5 * ELEMENTS; i++)
//	{
//		uint32_table.Set(i, new std::uint32_t(i));
//		
//		if (index == ELEMENTS + 1)
//			index = 1;
//
//		c++;
//	}
//
////	print("Producent " + to_string(num) + ": " + to_string(c));
//}
//
//void HashGet(int num)
//{
//	int c = 0;
//	int index = 1;
//	for (uint32_t i = 1; i <= 10 * ELEMENTS; i++)
//	{
//		auto x = uint32_table.Get(index++);
//
//		if (index == ELEMENTS + 1)
//			index = 1;
//
//		c++;
//	}
//
////	print("Consument " + to_string(num) + ": " + to_string(c));
//}
//
////void HashSetStr(int num)
////{
////	int c = 0;
////	int index = 1;
////	for (uint32_t i = 1; i <= 5 * ELEMENTS; i++)
////	{
////		uint32_table.SetElement(index++, i);
////
////		if (index == ELEMENTS + 1)
////			index = 1;
////
////		c++;
////	}
////
////	print("Producent " + to_string(num) + ": " + to_string(c));
////}
////
////void HashGetStr(int num)
////{
////	int c = 0;
////	int index = 1;
////	for (uint32_t i = 1; i <= 10 * ELEMENTS; i++)
////	{
////		auto x = uint32_table.GetElement(index++);
////
////		if (index == ELEMENTS + 1)
////			index = 1;
////
////		c++;
////	}
////
////	print("Consument " + to_string(num) + ": " + to_string(c));
////}
//
//void testHASH(int producers, int consumers, void produce(int), void consume(int))
//{
//	using namespace std::chrono;
//
//	time_point<high_resolution_clock> start = high_resolution_clock::now();
//
//	std::vector<thread> consumersThreads;
//	std::vector<thread> producersThreads;
//
//
//
//
//	// tworzenie w¹tków producenta
//	for (int i = 0; i < producers; i++)
//		producersThreads.push_back(thread(produce, i));
//
//	// tworzenie w¹tków konsumenta
//	for (int i = 0; i < consumers; i++)
//		consumersThreads.push_back(thread(consume, i));
//
//	for (int i = 0; i < producers; i++)
//		producersThreads[i].join();
//
//	for (int i = 0; i <consumers; i++)
//		consumersThreads[i].join();
//
//	time_point<high_resolution_clock> stop = high_resolution_clock::now();
//
//	std::cout << "Time: " << duration<double>(stop - start).count() << std::endl;
//
//}


//struct A
//{
//	int x;
//};
//
//struct B
//{
//	B(const B&) {}
//};
//
//struct C
//{
//	virtual void foo();
//};
//
//struct D : public A
//{
//	int y;
//	bool z;
//	double v;
//	char c;
//};
//
//struct E
//{
//	A a;
//	D d;
//};

int main()
{

	
	//testHM::testInsert(100, 1000);
	//testHM::testGet(100, 1000);
	//testHM::printSizes();
	testHM::testAll(10, 10, 1000);
	testHM::testAll(10, 10, 1000);
	testHM::testAll(10, 10, 1000);
	//testHM::printSizes();
	//std::cout << std::boolalpha;
	//std::cout << std::is_trivially_copyable<A>::value << endl;
	//std::cout << std::is_trivially_copyable<B>::value << endl;
	//std::cout << std::is_trivially_copyable<C>::value << endl;
	//std::cout << std::is_trivially_copyable<D>::value << endl;
	//std::cout << std::is_trivially_copyable<E>::value << endl;
	//cout << endl;
	//std::cout << std::is_trivial<A>::value << endl;
	//std::cout << std::is_trivial<B>::value << endl;
	//std::cout << std::is_trivial<C>::value << endl;
	//std::cout << std::is_trivial<D>::value << endl;
	//std::cout << std::is_trivial<E>::value << endl;












	//mainTests();


	//testHASH(PRODUCERS, CONSUMERS, HashSet, HashGet);


	//hmtest::samples<std::uint32_t, std::uint32_t> sample(2,10000,0);
	//a = false;
	//b = false;
	//c = 0;
	//std::thread t1(aWrite_bWrite);
	//std::thread t2(bRead_aRead);
	////std::thread t3(aRead_bRead);
	////std::thread t4(bRead_aRead);
	//t1.join();
	//t2.join();
	////t3.join();
	////t4.join();
	//cout << c <<endl;
	//assert(c.load() != 0);

	//std::thread t1(create);
	//std::thread t2(use);
	//t1.join();
	//t2.join();

	//LockFreeHashTable<int, 1024> ht;
	//int* value = new int(13);
	//ht.Set(10, value);
	//auto result = ht.Get(10);

	//if (result == nullptr)
	//	cout << "NULL" << endl;
	//else
	//	cout << *result << endl;

	cin.get();
	return 0;
	//testCounterSpeed(5, 1000, 1000);
	
	
}