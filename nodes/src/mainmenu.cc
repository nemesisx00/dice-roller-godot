#include <mainmenu.hh>

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/os.hpp>

using namespace godot;

void MainMenu::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("pressHandler_godotLicense"), &MainMenu::pressHandler_godotLicense);
	ClassDB::bind_method(D_METHOD("pressHandler_quit"), &MainMenu::pressHandler_quit);
	ClassDB::bind_method(D_METHOD("pressHandler_start"), &MainMenu::pressHandler_start);
}

void MainMenu::_ready()
{
	isEditor = Engine::get_singleton()->is_editor_hint();
	
	if(!isEditor)
	{
		get_node<Button>("%GodotLicense")->connect("pressed", Callable(this, "pressHandler_godotLicense"));
		get_node<Button>("%Quit")->connect("pressed", Callable(this, "pressHandler_quit"));
		get_node<Button>("%Start")->connect("pressed", Callable(this, "pressHandler_start"));
	}
}

void MainMenu::pressHandler_godotLicense()
{
	OS::get_singleton()->shell_open("https://godotengine.org/license/");
}

void MainMenu::pressHandler_quit()
{
	get_tree()->quit();
}

void MainMenu::pressHandler_start()
{
	get_tree()->change_scene_to_file("res://Main.tscn");
}
