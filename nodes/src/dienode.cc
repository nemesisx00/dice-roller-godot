#include <dienode.hh>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/label3d.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/transform3d.hpp>
#include <direction.hh>
#include <string>

using namespace godot;

void DieNode::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("getValue"), &DieNode::getValue);
	ClassDB::bind_method(D_METHOD("handleSleepingStateChange"), &DieNode::handleSleepingStateChange);
	
	ClassDB::bind_method(D_METHOD("getSides"), &DieNode::getSides);
	ClassDB::bind_method(D_METHOD("setSides", "p_sides"), &DieNode::setSides);
	ClassDB::add_property("DieNode", PropertyInfo(Variant::INT, "Sides"), "setSides", "getSides");
	
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
	
	Vector3 up(0, 1, 0);
	TypedArray<Node> children = get_node<Node3D>("%Sides")->get_children();
	int childrenSize = children.size();
	for(int i = 0; i < childrenSize; i++)
	{
		Direction *child = Node::cast_to<Direction>(children[i]);
		if(child->getDirectionUnit() == up)
		{
			value = child->getValue();
			break;
		}
	}
	
	return value;
}

void DieNode::handleSleepingStateChange()
{
	if(is_sleeping())
	{
		updateDisplayLabel();
		emit_signal("asleep", get_instance_id());
	}
}

void DieNode::updateDisplayLabel()
{
	Label3D *label = get_node<Label3D>("%DisplayValue");
	
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
