#ifndef ROLLRESULT_HH
#define ROLLRESULT_HH

#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <algorithm>
#include <functional>
#include <random>
#include <vector>

namespace godot
{
	enum RollResultFields
	{
		Highest,
		Lowest,
		Sides,
		Total,
		Values,
	};
	
	struct RollResult
	{
		int highest;
		int lowest;
		int sides;
		int total;
		std::vector<int> values;
		
		RollResult(const int sides, const std::vector<int> values)
		{
			std::vector<int> sorted(values.size());
			std::copy(values.begin(), values.end(), sorted.begin());
			std::sort(sorted.begin(), sorted.end(), std::less<int>());
			
			auto total = 0;
			for(auto val : sorted)
				total += val;
			
			this->highest = sorted.back();
			this->lowest = sorted.front();
			this->sides = sides;
			this->total = total;
			this->values = values;
		}
		
		RollResult(const Dictionary dict)
		{
			if(dict.has(RollResultFields::Highest))
				highest = dict[RollResultFields::Highest];
			if(dict.has(RollResultFields::Lowest))
				lowest = dict[RollResultFields::Lowest];
			if(dict.has(RollResultFields::Sides))
				sides = dict[RollResultFields::Sides];
			if(dict.has(RollResultFields::Total))
				total = dict[RollResultFields::Total];
			
			if(dict.has(RollResultFields::Values))
			{
				TypedArray<int> received = dict[RollResultFields::Values];
				auto size = received.size();
				for(int64_t j = 0; j < size; j++)
				{
					values.push_back(received[j]);
				}
			}
		}
		
		Dictionary into() const
		{
			Dictionary dict;
			dict[RollResultFields::Highest] = highest;
			dict[RollResultFields::Lowest] = lowest;
			dict[RollResultFields::Sides] = sides;
			dict[RollResultFields::Total] = total;
			
			TypedArray<int> valueArray;
			for(auto val : values)
				valueArray.append(val);
			
			dict[RollResultFields::Values] = valueArray;
			
			return dict;
		}
	};
}

#endif // ROLLRESULT_HH
