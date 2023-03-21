#include <direction.hh>
#include <godot_cpp/core/property_info.hpp>

using namespace godot;

void Direction::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("getEndpoint"), &Direction::getEndpoint);
	ClassDB::bind_method(D_METHOD("setEndpoint", "p_point"), &Direction::setEndpoint);
	ClassDB::add_property("Direction", PropertyInfo(Variant::VECTOR3, "Endpoint"), "setEndpoint", "getEndpoint");
	
	ClassDB::bind_method(D_METHOD("getValue"), &Direction::getValue);
	ClassDB::bind_method(D_METHOD("setValue", "p_value"), &Direction::setValue);
	ClassDB::add_property("Direction", PropertyInfo(Variant::INT, "Value"), "setValue", "getValue");
	
	ClassDB::bind_method(D_METHOD("getDirection"), &Direction::getDirection);
	ClassDB::bind_method(D_METHOD("getDirectionUnit"), &Direction::getDirectionUnit);
}

Vector3 Direction::getDirection() const
{
	return get_global_transform()
		.basis
		.xform(Endpoint);
}

Vector3 Direction::getDirectionUnit() const
{
	return getDirection()
		.normalized();
}
