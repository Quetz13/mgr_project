#pragma once
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> a, b;
std::atomic<int> c;

void aWrite()
{
	a.store(true, std::memory_order_seq_cst);
}

void bWrite()
{
	b.store(true, std::memory_order_seq_cst);
}

void aRead_bRead()
{
	while (!a.load(std::memory_order_seq_cst));
		if (b.load(std::memory_order_seq_cst))
			++c;
}

void aWrite_bWrite()
{
	a.store(true, std::memory_order_relaxed);
	b.store(true, std::memory_order_relaxed);
}

void bRead_aRead()
{
	while (!b.load(std::memory_order_relaxed));
	if (a.load(std::memory_order_relaxed))
		++c;
}



///////CONSUME

std::atomic<std::string*> stringPtr;
std::atomic<int> intValue;

void create()
{
	std::string* p = new std::string("Test");
	intValue.store(69, std::memory_order_relaxed);
	stringPtr.store(p, std::memory_order_release);
}

void use()
{
	std::string* p;
	while (!(p = stringPtr.load(std::memory_order_consume)));
	assert(*p == "Test");
	assert(intValue == 69);
}



///////REL_ACQ

//std::vector<int> data;
//std::atomic<int> flag = { 0 };
//
//void thread1()
//{
//	data.push_back(69);
//	flag.store(1, std::memory_order_release);
//}
//
//void thread2()
//{
//	int expected = 1;
//	while (!flag.compare_exchange_strong(expected, 2, std::memory_order_acq_rel))
//		expected = 1;
//}
//
//void thread3()
//{
//	while (flag.load(std::memory_order_acquire) < 2);
//	assert(data.at(0) == 42);
//}
//
//int main()
//{
//	std::thread t1(thread1);
//	std::thread t2(thread2);
//	std::thread t3(thread3);
//	t1.join(); 
//	t2.join(); 
//	t3.join();
//}