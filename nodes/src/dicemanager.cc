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
	
	ClassDB::add_property_group("DiceManager", "Impulse at Spawn", "");
	
	ClassDB::bind_method(D_METHOD("getGroupImpulse"), &DiceManager::getGroupImpulse);
	ClassDB::bind_method(D_METHOD("setGroupImpulse", "p_min"), &DiceManager::setGroupImpulse);
	ClassDB::add_property("DiceManager", PropertyInfo(Variant::BOOL, "Group Impulse by Die Type"), "setGroupImpulse", "getGroupImpulse");
	
	ClassDB::bind_method(D_METHOD("getDownforceMax"), &DiceManager::getDownforceMax);
	ClassDB::bind_method(D_METHOD("setDownforceMax", "p_max"), &DiceManager::setDownforceMax);
	ClassDB::add_property("DiceManager", PropertyInfo(Variant::FLOAT, "Maximum Downforce"), "setDownforceMax", "getDownforceMax");
		
	ClassDB::bind_method(D_METHOD("getDownforceMin"), &DiceManager::getDownforceMin);
	ClassDB::bind_method(D_METHOD("setDownforceMin", "p_min"), &DiceManager::setDownforceMin);
	ClassDB::add_property("DiceManager", PropertyInfo(Variant::FLOAT, "Minimum Downforce"), "setDownforceMin", "getDownforceMin");
	
	ClassDB::bind_method(D_METHOD("getForceMax"), &DiceManager::getForceMax);
	ClassDB::bind_method(D_METHOD("setForceMax", "p_max"), &DiceManager::setForceMax);
	ClassDB::add_property("DiceManager", PropertyInfo(Variant::FLOAT, "Maximum Lateral Force"), "setForceMax", "getForceMax");
	
	ClassDB::bind_method(D_METHOD("getForceMin"), &DiceManager::getForceMin);
	ClassDB::bind_method(D_METHOD("setForceMin", "p_min"), &DiceManager::setForceMin);
	ClassDB::add_property("DiceManager", PropertyInfo(Variant::FLOAT, "Minimum Lateral Force"), "setForceMin", "getForceMin");
	
	ClassDB::add_property_group("DiceManager", "Transform at Spawn", "");
	
	ClassDB::bind_method(D_METHOD("getPositionMax"), &DiceManager::getPositionMax);
	ClassDB::bind_method(D_METHOD("setPositionMax", "p_max"), &DiceManager::setPositionMax);
	ClassDB::add_property("DiceManager", PropertyInfo(Variant::FLOAT, "Position Maximum"), "setPositionMax", "getPositionMax");
	
	ClassDB::bind_method(D_METHOD("getPositionMin"), &DiceManager::getPositionMin);
	ClassDB::bind_method(D_METHOD("setPositionMin", "p_min"), &DiceManager::setPositionMin);
	ClassDB::add_property("DiceManager", PropertyInfo(Variant::FLOAT, "Position Minimum"), "setPositionMin", "getPositionMin");
			
	ClassDB::bind_method(D_METHOD("getRotationMax"), &DiceManager::getRotationMax);
	ClassDB::bind_method(D_METHOD("setRotationMax", "p_max"), &DiceManager::setRotationMax);
	ClassDB::add_property("DiceManager", PropertyInfo(Variant::FLOAT, "Rotation Max"), "setRotationMax", "getRotationMax");
	
	ClassDB::bind_method(D_METHOD("getRotationMin"), &DiceManager::getRotationMin);
	ClassDB::bind_method(D_METHOD("setRotationMin", "p_min"), &DiceManager::setRotationMin);
	ClassDB::add_property("DiceManager", PropertyInfo(Variant::FLOAT, "Rotation Min"), "setRotationMin", "getRotationMin");
	
	ClassDB::bind_method(D_METHOD("getSpawnHeight"), &DiceManager::getSpawnHeight);
	ClassDB::bind_method(D_METHOD("setSpawnHeight", "p_height"), &DiceManager::setSpawnHeight);
	ClassDB::add_property("DiceManager", PropertyInfo(Variant::FLOAT, "Spawn Height"), "setSpawnHeight", "getSpawnHeight");
}

void DiceManager::_ready()
{
	isEditor = Engine::get_singleton()->is_editor_hint();
	
	if(!isEditor)
	{
		auto logic = get_parent()->get_node<UILogic>("UI/UILogic");
		logic->connect("clear_dice", Callable(this, "handleClearDice"));
		logic->connect("spawn_dice", Callable(this, "handleSpawnDice"));
	}
}

void DiceManager::handleAsleep(const uint64_t id)
{
	auto it = std::find(nodeIds.begin(), nodeIds.end(), id);
	
	if(it != nodeIds.end())
		nodeIds.erase(it);
	
	if(nodeIds.size() < 1)
		processResults();
}

void DiceManager::handleClearDice()
{
	auto children = get_children();
	auto childrenSize = children.size();
	for(int64_t i = 0; i < childrenSize; i++)
	{
		auto node = Node::cast_to<Node>(children[i]);
		node->queue_free();
	}
	nodeIds.clear();
}

void DiceManager::handleSpawnDice(const int sides, const int quantity = 1)
{
	if(quantity > 0)
	{
		String path("res://dice/d{0}.tscn");
		Array params;
		Ref<PackedScene> resource;
		switch(sides)
		{
			case DiceType::Four:
				params.push_back(DiceType::Four);
				break;
			case DiceType::Six:
				params.push_back(DiceType::Six);
				break;
			case DiceType::Eight:
				params.push_back(DiceType::Eight);
				break;
			case DiceType::Ten:
				params.push_back(DiceType::Ten);
				break;
			case DiceType::Twelve:
				params.push_back(DiceType::Twelve);
				break;
			case DiceType::Twenty:
				params.push_back(DiceType::Twenty);
				break;
			case DiceType::Hundred:
				params.push_back(DiceType::Hundred);
				break;
		}
		
		if(params.size() > 0)
			resource = ResourceLoader::get_singleton()->load(path.format(params));
		
		if(resource->can_instantiate())
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> pos(PositionMin, PositionMax);
			std::uniform_real_distribution<> rot(RotationMin, RotationMax);
			std::uniform_real_distribution<> force(ForceMin, ForceMax);
			std::uniform_real_distribution<> dforce(DownforceMin, DownforceMax);
			
			Vector3 x(1, 0, 0);
			Vector3 y(0, 1, 0);
			Vector3 z(0, 0, 1);
			
			Vector3 impulse(force(gen), dforce(gen), force(gen));
			
			for(int i = 0; i < quantity; i++)
			{
				auto die = Node::cast_to<DieNode>(resource->instantiate());
				die->translate(Vector3(pos(gen), SpawnHeight, pos(gen)));
				die->rotate_object_local(x, rot(gen));
				die->rotate_object_local(y, rot(gen));
				die->rotate_object_local(z, rot(gen));
				die->connect("asleep", Callable(this, "handleAsleep"));
				nodeIds.push_back(die->get_instance_id());
				
				add_child(die);
				
				if(GroupImpulse)
					die->apply_impulse(impulse);
				else
					die->apply_impulse(Vector3(force(gen), dforce(gen), force(gen)));
			}
		}
	}
}

void DiceManager::processResults()
{
	std::map<int, std::vector<int>> diceValues;
	auto children = get_children();
	auto childrenSize = children.size();
	for(int64_t i = 0; i < childrenSize; i++)
	{
		auto die = Node::cast_to<DieNode>(children[i]);
		diceValues[die->getSides()].push_back(die->getValue());
	}
	
	for(auto pair : diceValues)
	{
		RollResult result(pair.first, pair.second);
		UtilityFunctions::print(result.values.size(), "d", result.sides, " Total: ", result.total);
	}
	
	emit_signal("roll_completed");
}
