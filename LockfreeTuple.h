#pragma once
#include <atomic>


class LockFreeTuple
{
public:
	LockFreeTuple()
	{

	}

	LockFreeTuple(const LockFreeTuple& tuple)
	{

	}

	const LockFreeTuple& operator=(const LockFreeTuple& tuple)
	{

	}



	~LockFreeTuple()
	{

	}

private:
	std::atomic<long long> _storage;
};