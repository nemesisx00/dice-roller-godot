#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <dicemanager.hh>
#include <dienode.hh>
#include <direction.hh>
#include <hellocpp.hh>
#include <mainmenu.hh>
#include <uilogic.hh>

using namespace godot;

void registerNodes(godot::ModuleInitializationLevel p_level)
{
	if(p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		ClassDB::register_class<DiceManager>();
		ClassDB::register_class<DieNode>();
		ClassDB::register_class<Direction>();
		ClassDB::register_class<HelloCpp>();
		ClassDB::register_class<MainMenu>();
		ClassDB::register_class<UILogic>();
	}
}

void unregisterNodes(godot::ModuleInitializationLevel p_level)
{
	if(p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		//
	}
}

extern "C"
{
	GDExtensionBool GDE_EXPORT godotCppInit(const GDExtensionInterface *p_interface, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
	{
		GDExtensionBinding::InitObject object(p_interface, p_library, r_initialization);
		object.register_initializer(registerNodes);
		object.register_terminator(unregisterNodes);
		object.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
		return object.init();
	}
}
