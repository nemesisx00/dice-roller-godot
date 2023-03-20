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
		std::vector<int> sorted(values.size());
		std::copy(values.begin(), values.end(), sorted.begin());
		std::sort(sorted.begin(), sorted.end(), std::less<int>());
		
		auto total = 0;
		for(auto val : sorted)
			total += val;
		
		this->average = std::round(total / sorted.size());
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
