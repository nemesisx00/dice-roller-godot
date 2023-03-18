#ifndef ROLLRESULT_HH
#define ROLLRESULT_HH

#include <algorithm>
#include <functional>
#include <random>
#include <vector>

struct RollResult
{
	RollResult(const int sides, const std::vector<int> values)
	{
		std::vector<int> sorted;
		std::copy(values.begin(), values.end(), sorted.begin());
		std::sort(sorted.begin(), sorted.end(), std::less<int>());
		
		int total = 0;
		int sortedSize = sorted.size();
		for(int i = 0; i < sortedSize; i++)
		{
			total += sorted[i];
		}
		
		this->average = std::round(total / sortedSize);
		this->highest = sorted.back();
		this->lowest = sorted.front();
		this->sides = sides;
		this->total = total;
		this->values = values;
	}
	
	int average;
	int highest;
	int lowest;
	int sides;
	int total;
	std::vector<int> values;
};

#endif // ROLLRESULT_HH
