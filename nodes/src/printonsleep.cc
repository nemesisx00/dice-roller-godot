#include <printonsleep.hh>

#include <godot_cpp/variant/utility_functions.hpp>
#include <dienode.hh>

using namespace godot;

PrintOnSleep::PrintOnSleep()
{
	hasPrinted = false;
	current = 0;
	limit = 10;
}

void PrintOnSleep::_bind_methods() {}

void PrintOnSleep::_ready()
{
	hasPrinted = false;
	current = 0;
}

void PrintOnSleep::_physics_process(double delta)
{
	if(current < limit && !hasPrinted && is_sleeping())
	{
		int value = get_node<DieNode>("DieNode")->getValue();
		if(value > 0)
		{
			UtilityFunctions::print("Value: ", value);
			hasPrinted = true;
		}
		current++;
	}
}
