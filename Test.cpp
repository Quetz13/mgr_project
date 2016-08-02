#include "Test.h"
#include <iostream>
#include <string>
#include "newnewFIFO.h"

using namespace std;

bool done = false;

//NewFIFOQueue<int, CONSUMERS> newFIFOQueue;
NewNewFIFO<int> newFIFOQueue;

BlockingFIFO<int, CONSUMERS> blockingFIFO;


void print(const std::string& text)
{
	static mutex m;
	m.lock();
	cout << text << endl;
	m.unlock();
}

void NewFIFOQueue_produce(int num)
{
	int c = 0;
	for (int i = 0; i < ELEMENTS; i++)
	{
		newFIFOQueue.Produce(i);
		c++;
	}

	print("Producent " + to_string(num) + ": " + to_string(c));	
}

void NewFIFOQueue_consume(int num)
{
	int x = 0;
	int c = 0;

	//newFIFOQueue.AddConsument();

	while (!done)
	{
		if(newFIFOQueue.Consume(x) == 1)
			c++;
		//this_thread::sleep_for(std::chrono::milliseconds(1));

	}

	int result = false;
	do
	{
		result = newFIFOQueue.Consume(x);
		if (result == 1)
			c++;
		//this_thread::sleep_for(std::chrono::milliseconds(1));
	} while (result != 0);

	print("Consument " + to_string(num) + ": " + to_string(c));
}

void BlockingFIFO_produce(int num)
{
	int c = 0;
	for (int i = 0; i < ELEMENTS; i++)
	{
		blockingFIFO.Produce(i);
		c++;
	}
	print("Producent " + to_string(num) + ": " + to_string(c));
		
}

void BlockingFIFO_consume(int num)
{
	int x = 0;
	int c = 0;

	blockingFIFO.AddConsument();

	while (!done)
	{
		blockingFIFO.Consume(x);
		c++;
	}

	int result = 0;
	do
	{
		result = blockingFIFO.Consume(x);
		if(result = 0)
			c++;
	} while (result != 0);

	print("Consument " + to_string(num) + ": " + to_string(c));
}


void testFIFO(int producers, int consumers, void produce(int), void consume(int))
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

	done = true;

	for (int i = 0; i <consumers; i++)
		consumersThreads[i].join();
		
	time_point<high_resolution_clock> stop = high_resolution_clock::now();

	std::cout << "Time: " << duration<double>(stop - start).count() << std::endl;

}


void mainTests()
{
	cout << "NewFIFOQueue: " << endl;
	testFIFO(PRODUCERS, CONSUMERS, NewFIFOQueue_produce, NewFIFOQueue_consume);
	cout << endl;

	//cout << "BlockingFIFO: " << endl;
	//testFIFO(PRODUCERS, CONSUMERS, BlockingFIFO_produce, BlockingFIFO_consume);
	//cout << endl;
}