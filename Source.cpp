#include "Test.h"
#include <chrono>
#include <string>
#include <iostream>
#include <thread>
#include <vector>

#include "HashTable.h"

////BlockingCounter counter;
////LockFreeCounter counter;
//WaitFreeCounter counter;
//
//
//void incrementCounter(int loopCounter)
//{
//	int x = 0;
//	for (int i = 0; i < loopCounter; i++)
//	{
//		//counter.increment();
//		x = counter.get();
//	}
//}
//
//void testCounterSpeed(int testsAmount, int threadsAmounts, int incrementsAmount)
//{
//	using namespace std::chrono;
//
//	for (int i = 0; i < testsAmount; i++)
//	{
//		time_point<high_resolution_clock> start = high_resolution_clock::now();
//
//		std::vector<std::thread*> threads;
//
//		for (int j = 0; j < threadsAmounts; j++)
//			threads.push_back(new std::thread(&incrementCounter, incrementsAmount));
//
//		for (int i = 0; i < threads.size(); i++)
//			threads[i]->join();
//
//		time_point<high_resolution_clock> stop = high_resolution_clock::now();
//
//		std::cout << "Time: " << duration<double>(stop - start).count() << std::endl;
//	}
//
//}
//
//
//struct ok
//{
//	int8_t x1;
//	int8_t x2;
//	int8_t x3;
//	int8_t x4;
//	int8_t x5;
//	int8_t x6;
//	int8_t x7;
//	int8_t x8;
//	//int64_t x2;
//	//char x2[32];
//
//};
//std::mutex m;
//void console(const std::string& text)
//{
//	m.lock();
//	std::cout << text << std::endl;
//	m.unlock();
//}
//
//NewFIFOQueue<int, CONSUMERS> fifo;
//std::atomic<bool> done = false;
//
//void produce(int num)
//{
//	int c = 0;
//	for (int i = 0; i < 10; i++)
//	{
//		fifo.TryProduce(i);
//		c++;
//		//console(std::to_string(num) + ": produce " + std::to_string(i));
//	}
//	console(std::to_string(num) + ": produced " + std::to_string(c));
//}
//
//void consume(int num)
//{
//	int x = 0;
//	int c = 0;
//
//	fifo.AddConsument();
//
//	while (!done)
//	{
//		if (fifo.Consume(x) == 1)
//			c++;//console("\t" + std::to_string(num) + ": consume " + std::to_string(x));
//	}
//
//	int result = 0;
//	do
//	{
//		result = fifo.Consume(x);
//		if (result == 1)
//			c++;
//	} while (result != 0);
//
//
//	console("\t" + std::to_string(num) + ": consumed " + std::to_string(c));
//}
//




int main()
{
	//using namespace std;

	//std::vector<thread> consumers;
	//std::vector<thread> producers;

	//for (int i = 0; i < CONSUMERS; i++)
	//{
	//	consumers.push_back(thread(consume, i));
	//}


	////for (int i = 0; i < 10; i++)
	////{
	//	producers.push_back(thread(produce, 0));
	////}



	////for (int i = 0; i < 10; i++)
	//	//producers[i].join();
	//producers[0].join();


	//done = true;

	//for (int i = 0; i <CONSUMERS; i++)
	//	consumers[i].join();

	//mainTests();

	HashTable<std::uint32_t, uint32_t_hash> table(20);
	table.SetElement(20, 3422);
	cout << table.GetElement(20) << endl;
	cout << table.GetElement(21) << endl;
	table.SetElement(20, 34422);
	cout << table.GetElement(20) << endl;
	cout << table.GetElement(21) << endl;

	cin.get();
	return 0;
	//testCounterSpeed(5, 1000, 1000);
	
	
}