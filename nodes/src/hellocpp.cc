#include <hellocpp.hh>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input.hpp>

namespace godot
{
	HelloCpp::HelloCpp()
	{
		initialText = "Hello, C++! Ready to go!";
	}
	
	void HelloCpp::_bind_methods() {}
	
	void HelloCpp::_ready()
	{
		Node::_ready();
		
		UtilityFunctions::print(initialText);
		set_text(initialText);
	}
}
