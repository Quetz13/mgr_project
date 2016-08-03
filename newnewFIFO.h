#pragma once
#include <atomic>
#include <limits>
#include <iostream>
#include <unordered_map>


using namespace std;

// NIEBLOKUJ¥CA KOLEJKA
template <typename T>
class NewNewFIFO {
private:
	struct Node
	{
		Node(T val) : value(val), next(nullptr) { }
		T value;
		//std::atomic<Node*> next;
		std::atomic<Node*> next;
		//std::atomic<unsigned int> counter;
	};

	//struct atomic_wrapper
	//{
	//	std::atomic<bool> value;

	//	atomic_wrapper() : value(false) {}
	//	atomic_wrapper(const std::atomic<bool> &f) : value(f.load()) {} // nie jest atomowy
	//	atomic_wrapper(const atomic_wrapper &other) :value(other.value.load()){}
	//	atomic_wrapper &operator=(const atomic_wrapper &other)
	//	{
	//		value.store(other.value.load());
	//	}
	//};

	Node* first;						// u¿ywany tylko przez producenta
	std::atomic<Node*> divider, last;        // u¿ywane przez producenta i konsumenta
	//std::atomic<unsigned int> size;
	//u¿ywa tylko producent + metoda z zewn¹trz
	//std::atomic<unsigned int> capacity;
	//std::atomic<bool> consumerLock;
	//std::unordered_map<size_t, atomic_wrapper> consumerFlags;

public:
	NewNewFIFO()
	{
		first = divider = last = new Node(T());				// dodanie separatora
	}

	~NewNewFIFO()
	{
		//while (first != nullptr)
		//{
		//	//Node* tmp = first;
		//	//first = tmp->next;
		//	//delete tmp;
		//}
	}

	//bool TryProduce(const T& t)
	//{
	//	if (capacity != std::numeric_limits<unsigned int>::max() && size >= capacity)
	//		return false;

	//	// dodanie nowego obiektu
	//	last.load()->next = new Node(t);
	//	// 'opublikowanie' nowego obiektu
	//	last = last.load()->next.load();
	//	// zwiêkszenie zmiennej oznaczaj¹cej wielkoœæ kolejki
	//	//size++;

	//	// usuniêcie nieu¿ywanych obiektów
	//	//while (first != divider)
	//	//{
	//	//	Node* tmp = first;
	//	//	first = first->next;
	//	//	delete tmp;
	//	//}

	//	return true;
	//}

	//void AddConsument()
	//{
	//	while (consumerLock.exchange(true));
	//	consumerFlags.insert(std::make_pair(std::this_thread::get_id().hash(), atomic_wrapper()));
	//	consumerLock = false;
	//}

	void Produce(const T& t)
	{
		//while (size >= capacity);

		// dodanie nowego obiektu
		last.load()->next = new Node(t);
		// 'opublikowanie' nowego obiektu
		last.store(last.load()->next);
		// zwiêkszenie zmiennej oznaczaj¹cej wielkoœæ kolejki
		//size++;

		//// usuniêcie nieu¿ywanych obiektów
		//while (first != divider)
		//{
		//	Node* tmp = first;
		//	first = first->next;
		//	delete tmp;
		//}
	}

	/// 0 - brak elementow do wziecia
	/// 1 - element wziety
	int Consume(T& result)
	{
		while (true)
		{
			

			// jeœli kolejka nie jest pusta
			if (divider.load() != last.load())
			{			
				
				std::atomic_thread_fence(std::memory_order_acquire);

				if (divider.load() == nullptr)
				{
					std::this_thread::yield();
					continue;
					//return 0;
				}
				assert(divider.load() != nullptr);
				
				Node* oldDivider = divider;
				std::atomic<Node*> oldNextDivider = divider.load()->next.load();
				

				//if (atomic_compare_exchange_strong_explicit(&oldNextDivider, &oldDivider, divider.load()->next.load(), memory_order_release, memory_order_relaxed))
				//{
				
					if (atomic_compare_exchange_strong_explicit(&divider, &oldDivider, oldNextDivider.load(), memory_order_release, memory_order_relaxed))
					{
						
						result = oldDivider->value;
						std::atomic_thread_fence(std::memory_order_release);
						//delete oldDivider;
						
						return 1;
					}
					//std::atomic_thread_fence(std::memory_order_release);
				//}
				//std::atomic_thread_fence(std::memory_order_release);
			}
			else
				return 0;
		}
	}

	//void SetCapacity(unsigned int maxCapacity)
	//{
	//	capacity = maxCapacity;
	//}

	//bool Size() const
	//{
	//	return size;
	//}

	//bool isLockFree() const
	//{
	//	if (divider.is_lock_free() && last.is_lock_free() && size.is_lock_free())
	//		return true;
	//	return false;
	//}

};
