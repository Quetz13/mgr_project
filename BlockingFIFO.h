#pragma once
#include <mutex>
#include <thread>
#include <queue>
#include <unordered_map>
#include <atomic>

// NIEBLOKUJ¥CA KOLEJKA
template <typename T, unsigned int consumers>
class BlockingFIFO {
private:
	struct atomic_wrapper
	{
		std::atomic<bool> value;

		atomic_wrapper() : value(false) {}
		atomic_wrapper(const std::atomic<bool> &f) : value(f.load()) {} // nie jest atomowy
		atomic_wrapper(const atomic_wrapper &other) :value(other.value.load()){}
		atomic_wrapper &operator=(const atomic_wrapper &other)
		{
			value.store(other.value.load());
		}
	};

	std::queue<T> _fifo;
	std::unordered_map<size_t, atomic_wrapper> consumerFlags;
	std::mutex _lock;
	unsigned int _counter;

public:
	BlockingFIFO() : _counter(0), _fifo()
	{
	}

	~BlockingFIFO()
	{
	}

	void AddConsument()
	{
		_lock.lock();
		consumerFlags.insert(std::make_pair(std::this_thread::get_id().hash(), atomic_wrapper()));
		_lock.unlock();
	}

	void Produce(const T& t)
	{
		_lock.lock();

		_fifo.push(t);

		_lock.unlock();
	}

	int Consume(T& result)
	{
		_lock.lock();

		if (consumerFlags.find(std::this_thread::get_id().hash())->second.value.load())
		{
			if (_fifo.empty())
			{
				_lock.unlock();
				return 0;
			}
			_lock.unlock();
			return -1;
		}
			
		if (_fifo.empty())
		{
			_lock.unlock();
			return 0;
		}
			

		result = _fifo.front();
		_counter++;
		consumerFlags.at(std::this_thread::get_id().hash()).value.store(true);

		if (_counter == consumers)
		{
			_fifo.pop();
			for (std::unordered_map<size_t, atomic_wrapper>::iterator it = consumerFlags.begin(); it != consumerFlags.end(); it++)
				it->second.value = false;
			_counter = 0;
		}

		_lock.unlock();
		return 1;
	}
};
