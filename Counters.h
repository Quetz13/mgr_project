    ///////////////////////////////////////////
   // @author	Bartosz Domaga³a			//
  // @mail		bardomagala@gmail.com	   //
 // @date		11.01.2016				  //
///////////////////////////////////////////

#include <atomic>
#include <mutex>

class BlockingCounter
{
public:
	BlockingCounter() : _counter(0)
	{
	}

	void increment()
	{
		_mutex.lock();
		_counter += 1;
		_mutex.unlock();
	}

	int get()
	{
		int temp;
		_mutex.lock();
		temp = _counter;
		_mutex.unlock();
		return _counter;	
	}

private:
	int _counter;
	std::mutex _mutex;
};

class LockFreeCounter
{
public:
	LockFreeCounter() : _counter(0)
	{
	}
	
	void increment()
	{
		int temp = _counter;
		while (!_counter.compare_exchange_weak(temp, temp + 1));
	}

	int get()
	{
		return _counter.load();
	}

private:
	std::atomic<int> _counter;
};


class WaitFreeCounter
{
public:
	WaitFreeCounter() : _counter(0)
	{
	}

	void increment()
	{
		_counter.fetch_add(1);
	}

	int get()
	{
		return _counter.load();
	}

private:
	std::atomic<int> _counter;
};