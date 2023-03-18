#include <dicemanager.hh>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <random>

#include <printonsleep.hh>
#include <uilogic.hh>

using namespace godot;

void DiceManager::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("handleSpawnDie"), &DiceManager::handleSpawnDie);
}

void DiceManager::_ready()
{
	isEditor = Engine::get_singleton()->is_editor_hint();
	
	if(!isEditor)
	{
		UILogic *logic = get_parent()->get_node<UILogic>("UI/UILogic");
		logic->connect("spawn_die", Callable(this, "handleSpawnDie"));
	}
}

void DiceManager::handleSpawnDie(const int sides, const int quantity = 1)
{
	UtilityFunctions::print("Spawn a die with ", sides, " sides!");
	
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
			
			Vector3 x(1, 0, 0);
			Vector3 y(0, 1, 0);
			Vector3 z(0, 0, 1);
			
			for(int i = 0; i < quantity; i++)
			{
				Node3D *die = Node::cast_to<Node3D>(resource->instantiate());
				die->translate(Vector3(pdist(gen), 25, pdist(gen)));
				die->rotate_object_local(x, rdist(gen));
				die->rotate_object_local(y, rdist(gen));
				die->rotate_object_local(z, rdist(gen));
				add_child(die);
			}
		}
	}
}
