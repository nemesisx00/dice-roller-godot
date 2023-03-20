#include <mainmenu.hh>

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/window.hpp>

using namespace godot;

void MainMenu::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("pressHandler_quit"), &MainMenu::pressHandler_quit);
	ClassDB::bind_method(D_METHOD("pressHandler_start"), &MainMenu::pressHandler_start);
}

void MainMenu::_ready()
{
	isEditor = Engine::get_singleton()->is_editor_hint();
	
	if(!isEditor)
	{
		get_node<Button>("%Quit")->connect("pressed", Callable(this, "pressHandler_quit"));
		get_node<Button>("%Start")->connect("pressed", Callable(this, "pressHandler_start"));
	}
}

void MainMenu::pressHandler_quit()
{
	get_tree()->quit();
}

void MainMenu::pressHandler_start()
{
	Ref<PackedScene> resource = ResourceLoader::get_singleton()->load("res://Main.tscn");
	if(resource->can_instantiate())
	{
		auto scene = Node::cast_to<Node3D>(resource->instantiate());
		auto tree = get_tree();
		
		tree->get_current_scene()->queue_free();
		tree->get_root()->add_child(scene);
		tree->set_current_scene(scene);
	}
}
