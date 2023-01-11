#pragma once
#include <random>
#include <iterator>

class Random
{
public:
	static float Random01()
	{
		return (float)dist(e2);
	}

	static int Random0n(int n)
	{
		std::uniform_int_distribution<> distribution(0, n);
		return distribution(e2);
	}

	template<class BidiIter >
	static BidiIter random_unique(BidiIter begin, BidiIter end, size_t num_random)
	{
		size_t left = std::distance(begin, end);
		while (num_random--)
		{
			BidiIter r = begin;
			std::advance(r, rand() % left);
			std::swap(*begin, *r);
			++begin;
			--left;
		}
		return begin;
	}

private:
	static std::mt19937 e2;
	static std::uniform_real_distribution<> dist;
};

