#include <dienode.hh>

#include <direction.hh>

using namespace godot;

void DieNode::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("getValue"), &DieNode::getValue);
}

void DieNode::_ready()
{
	sides = std::unique_ptr<Node3D>(get_parent()->get_node<Node3D>("%Sides"));
}

int DieNode::getValue() const
{
	int value = -1;
	
	Vector3 up(0, 1, 0);
	TypedArray<Node> children = sides->get_children();
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
