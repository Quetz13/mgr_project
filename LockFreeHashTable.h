#pragma once
#include <atomic>

template<typename T>
class Bucket
{
private:
	struct BucketElement
	{
		BucketElement(T val) : value(val), next(nullptr) {}
		T value;
		std::atomic<Node*> next;
	};

	std::atomic<BucketElement*> divider, last;
	BucketElement* first;

public:
	Bucket()
	{
		first = divider = last = new BucketElement(T());
	}

	~Bucket()
	{
		while (first != nullptr)
		{
			BucketElement* tmp = first;
			first = tmp->next;
			delete tmp;
		}
	}
	
	void push_back(T newElement)
	{
		last.load()->next = new BucketElement(newElement);
		last.store(last.load()->next);
	}
};