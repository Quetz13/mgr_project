#pragma once
#include <atomic>
#include <limits>
#include <iostream>
#include <unordered_map>


using namespace std;

// NIEBLOKUJ¥CA KOLEJKA
template <typename T, unsigned int consumers>
class NewFIFOQueue {
private:
	struct Node
	{
		Node(T val) : value(val), next(nullptr), counter(0) { }
		T value;
		std::atomic<Node*> next;
		std::atomic<unsigned int> counter;
	};
	
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

	Node* first;						// u¿ywany tylko przez producenta
	std::atomic<Node*> divider, last;        // u¿ywane przez producenta i konsumenta
	std::atomic<unsigned int> size;
	//u¿ywa tylko producent + metoda z zewn¹trz
	std::atomic<unsigned int> capacity;
	std::atomic<bool> consumerLock;
	std::unordered_map<size_t, atomic_wrapper> consumerFlags;

public:
	NewFIFOQueue() : size(0), capacity(std::numeric_limits<unsigned int>::max())
	{
		first = divider = last = new Node(T());				// dodanie separatora
	}

	~NewFIFOQueue() 
	{
		while (first != nullptr) 
		{
			Node* tmp = first;
			first = tmp->next;
			delete tmp;
		}
	}

	bool TryProduce(const T& t)
	{
		if (capacity != std::numeric_limits<unsigned int>::max() && size >= capacity)
			return false;

		// dodanie nowego obiektu
		last.load()->next = new Node(t);
		// 'opublikowanie' nowego obiektu
		last = last.load()->next.load();
		// zwiêkszenie zmiennej oznaczaj¹cej wielkoœæ kolejki
		size++;

		// usuniêcie nieu¿ywanych obiektów
		while (first != divider)
		{
			Node* tmp = first;
			first = first->next;
			delete tmp;
		}

		return true;
	}

	void AddConsument()
	{
		while (consumerLock.exchange(true));
		consumerFlags.insert(std::make_pair(std::this_thread::get_id().hash(), atomic_wrapper()));
		consumerLock = false;
	}

	void Produce(const T& t)
	{
		while (size >= capacity);

		// dodanie nowego obiektu
		last.load()->next = new Node(t);
		// 'opublikowanie' nowego obiektu
		last = last.load()->next;
		// zwiêkszenie zmiennej oznaczaj¹cej wielkoœæ kolejki
		size++;

		// usuniêcie nieu¿ywanych obiektów
		while (first != divider)
		{
			Node* tmp = first;
			first = first->next;
			delete tmp;
		}
	}

	/// 0 - brak elementow do wziecia
	/// 1 - element wziety
	/// -1 - element wczesniej przeczytany
	int Consume(T& result)
	{
		auto id = std::this_thread::get_id().hash();
		if (consumerFlags.find(id)->second.value.load())
			return -1;

		while(consumerLock.exchange(true));

		// jeœli kolejka nie jest pusta
		if (divider.load() != last.load()) 
		{
			result = divider.load()->next.load()->value;  // C: copy it back
			divider.load()->next.load()->counter++;
			consumerFlags.at(std::this_thread::get_id().hash()).value.store(true);

			if (divider.load()->next.load()->counter == consumers)
			{
				divider = divider.load()->next.load();   // D: publish that we took it
				size--;

				for (std::unordered_map<size_t, atomic_wrapper>::iterator it = consumerFlags.begin(); it != consumerFlags.end(); it++)
					it->second.value = false;
			}
			consumerLock = false;
			return 1;              // and report success
		}
		consumerLock = false;
		return 0;               // else report empty
	}

	void SetCapacity(unsigned int maxCapacity)
	{
		capacity = maxCapacity;
	}

	bool Size() const
	{
		return size;
	}

	bool isLockFree() const
	{
		if (divider.is_lock_free() && last.is_lock_free() && size.is_lock_free())
			return true;
		return false;
	}

};
