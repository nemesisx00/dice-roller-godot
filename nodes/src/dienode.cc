#include <dienode.hh>

#include <errno.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/environment.hpp>
#include <godot_cpp/classes/label3d.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/transform3d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/math.hpp>
#include <string>

#include <direction.hh>

using namespace godot;

void DieNode::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("getValue"), &DieNode::getValue);
	ClassDB::bind_method(D_METHOD("handleSleepingStateChange"), &DieNode::handleSleepingStateChange);
	
	ClassDB::bind_method(D_METHOD("getDisplayValue"), &DieNode::getDisplayValue);
	ClassDB::bind_method(D_METHOD("setDisplayValue", "p_path"), &DieNode::setDisplayValue);
	ClassDB::add_property("DieNode", PropertyInfo(Variant::NODE_PATH, "Display Value Node"), "setDisplayValue", "getDisplayValue");
	
	ClassDB::bind_method(D_METHOD("getDisplayValueLabel"), &DieNode::getDisplayValueLabel);
	ClassDB::bind_method(D_METHOD("setDisplayValueLabel", "p_display"), &DieNode::setDisplayValueLabel);
	ClassDB::add_property("DieNode", PropertyInfo(Variant::BOOL, "Display 3D Value"), "setDisplayValueLabel", "getDisplayValueLabel");
	
	ClassDB::bind_method(D_METHOD("getGuessNotFlat"), &DieNode::getGuessNotFlat);
	ClassDB::bind_method(D_METHOD("setGuessNotFlat", "p_guess"), &DieNode::setGuessNotFlat);
	ClassDB::add_property("DieNode", PropertyInfo(Variant::BOOL, "Guess When Not Flat"), "setGuessNotFlat", "getGuessNotFlat");
	
	ClassDB::bind_method(D_METHOD("getSides"), &DieNode::getSides);
	ClassDB::bind_method(D_METHOD("setSides", "p_sides"), &DieNode::setSides);
	ClassDB::add_property("DieNode", PropertyInfo(Variant::INT, "Sides"), "setSides", "getSides");
	
	ClassDB::bind_method(D_METHOD("getSidesPath"), &DieNode::getSidesPath);
	ClassDB::bind_method(D_METHOD("setSidesPath", "p_path"), &DieNode::setSidesPath);
	ClassDB::add_property("DieNode", PropertyInfo(Variant::NODE_PATH, "Sides Node"), "setSidesPath", "getSidesPath");
	
	ADD_SIGNAL(MethodInfo("asleep", PropertyInfo(Variant::INT, "p_id")));
}

void DieNode::_ready()
{
	isEditor = Engine::get_singleton()->is_editor_hint();
	
	if(!isEditor)
		connect("sleeping_state_changed", Callable(this, "handleSleepingStateChange"));
}

int DieNode::getValue() const
{
	int value = -1;
	
	Vector3 zero;
	Vector3 up(0, 1, 0);
	real_t biggest = -1;
	
	auto children = get_node<Node3D>(SidesPath)->get_children();
	auto childrenSize = children.size();
	for(int64_t i = 0; i < childrenSize; i++)
	{
		auto child = Node::cast_to<Direction>(children[i]);
		
		if(child->getDirectionUnit() == up || (!GuessNotFlat && child->getDirectionUnit().round() == up))
		{
			value = child->getValue();
			break;
		}
		else if(GuessNotFlat)
		{
			auto dotProduct = child->getDirection().dot(up);
			if(dotProduct > biggest)
			{
				biggest = dotProduct;
				value = child->getValue();
			}
		}
	}
	
	return value;
}

void DieNode::handleSleepingStateChange()
{
	if(is_sleeping())
	{
		if(DisplayValueLabel)
			updateDisplayLabel();
		emit_signal("asleep", get_instance_id());
	}
}

void DieNode::updateDisplayLabel()
{
	auto label = get_node<Label3D>(DisplayValuePath);
	
	Array params;
	int value = getValue();
	if(value > 0)
		params.push_front(value);
	else
		params.push_front("?");
	
	String valueFormat("{0}");
	label->set_text(valueFormat.format(params));
	label->set_global_position(get_global_position() + Vector3(0, 3, 0));
	label->set_global_rotation(Vector3(-1.5, 0, 0));
	label->show();
}
