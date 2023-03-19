#include <dicemanager.hh>

#include <algorithm>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/node_path.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <random>

#include <dienode.hh>
#include <rollresult.hh>
#include <stdint.h>
#include <uilogic.hh>

using namespace godot;

void DiceManager::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("handleAsleep", "p_id"), &DiceManager::handleAsleep);
	ClassDB::bind_method(D_METHOD("handleClearDice"), &DiceManager::handleClearDice);
	ClassDB::bind_method(D_METHOD("handleSpawnDice"), &DiceManager::handleSpawnDice);
	
	ADD_SIGNAL(MethodInfo("roll_completed"));
}

void DiceManager::_ready()
{
	isEditor = Engine::get_singleton()->is_editor_hint();
	
	if(!isEditor)
	{
		UILogic *logic = get_parent()->get_node<UILogic>("UI/UILogic");
		logic->connect("clear_dice", Callable(this, "handleClearDice"));
		logic->connect("spawn_dice", Callable(this, "handleSpawnDice"));
	}
}

void DiceManager::handleAsleep(const uint64_t id)
{
	std::list<uint64_t>::iterator it;
	it = std::find(nodeIds.begin(), nodeIds.end(), id);
	
	if(it != nodeIds.end())
		nodeIds.erase(it);
	
	if(nodeIds.size() < 1)
		processResults();
}

void DiceManager::handleClearDice()
{
	TypedArray<Node> children = get_children();
	int childrenSize = children.size();
	for(int i = 0; i < childrenSize; i++)
	{
		Node *node = Node::cast_to<Node>(children[i]);
		node->queue_free();
	}
	nodeIds.clear();
}

void DiceManager::handleSpawnDice(const int sides, const int quantity = 1)
{
	if(quantity > 0)
	{
		Ref<PackedScene> resource;
		switch(sides)
		{
			case 6:
				resource = ResourceLoader::get_singleton()->load("res://dice/d6.tscn");
				break;
			case 8:
				resource = ResourceLoader::get_singleton()->load("res://dice/d8.tscn");
				break;
		}
		
		if(resource->can_instantiate())
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> pdist(-5, 5);
			std::uniform_real_distribution<> rdist(-1.5, 1.5);
			std::uniform_real_distribution<> fdist(-25, 25);
			
			Vector3 x(1, 0, 0);
			Vector3 y(0, 1, 0);
			Vector3 z(0, 0, 1);
			
			for(int i = 0; i < quantity; i++)
			{
				DieNode *die = Node::cast_to<DieNode>(resource->instantiate());
				die->translate(Vector3(pdist(gen), 20, pdist(gen)));
				die->rotate_object_local(x, rdist(gen));
				die->rotate_object_local(y, rdist(gen));
				die->rotate_object_local(z, rdist(gen));
				die->connect("asleep", Callable(this, "handleAsleep"));
				nodeIds.push_back(die->get_instance_id());
				add_child(die);
				die->apply_impulse(Vector3(fdist(gen), 0, fdist(gen)));
			}
		}
	}
}

void DiceManager::processResults()
{
	std::map<int, std::vector<int>> diceValues;
	TypedArray<Node> children = get_children();
	int childrenSize = children.size();
	for(int i = 0; i < childrenSize; i++)
	{
		DieNode *die = Node::cast_to<DieNode>(children[i]);
		diceValues[die->getSides()].push_back(die->getValue());
	}
	
	for(std::map<int, std::vector<int>>::iterator it = diceValues.begin(); it != diceValues.end(); ++it)
	{
		RollResult result(it->first, it->second);
		UtilityFunctions::print(result.values.size(), "d", result.sides, " Total: ", result.total);
	}
	
	emit_signal("roll_completed");
}
