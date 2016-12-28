//#pragma once
//#include <atomic>
//
//struct pointer
//{
//	std::atomic<node*> _node;
//	std::atomic<unsigned int> _counter;
//
//};
//
//struct node
//{
//	int _data;
//	std::atomic<pointer*> _next;
//};
//
//struct queue 
//{
//	queue()
//	{
//		node* newNode = new node();
//		newNode->_next = nullptr;
//		_head.load()->_node = _tail.load()->_node = newNode;
//	}
//
//	void add(int data)
//	{
//		node* newNode = new node();
//		newNode->_data = data;
//		newNode->_next = nullptr;
//
//		while (true)
//		{
//			pointer* tail = _tail;
//			pointer* next = tail._node->_next;
//
//			if (tail == _tail)
//			{
//				if (next->_node == nullptr)
//				{
//					if (std::atomic_compare_exchange_strong(tail->_node->_next, next, ))
//						break;
//				}
//				else
//				{
//					std::atomic_compare_exchange_strong(_tail, tail, next, )
//				}
//			}
//		}
//		std::atomic_compare_exchange_strong(_tail, tail, next, )
//
//	}
//
//
//	std::atomic<pointer*> _head;
//	std::atomic<pointer*> _tail;
//};