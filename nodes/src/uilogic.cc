#include <godot_cpp/core/property_info.hpp>
#include <uilogic.hh>

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/check_button.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp//variant/dictionary.hpp>
#include <dienode.hh>

using namespace godot;

void UILogic::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("pressHandler_d4"), &UILogic::pressHandler_d4);
	ClassDB::bind_method(D_METHOD("pressHandler_d6"), &UILogic::pressHandler_d6);
	ClassDB::bind_method(D_METHOD("pressHandler_d8"), &UILogic::pressHandler_d8);
	ClassDB::bind_method(D_METHOD("pressHandler_d10"), &UILogic::pressHandler_d10);
	ClassDB::bind_method(D_METHOD("pressHandler_d12"), &UILogic::pressHandler_d12);
	ClassDB::bind_method(D_METHOD("pressHandler_d20"), &UILogic::pressHandler_d20);
	ClassDB::bind_method(D_METHOD("pressHandler_d100"), &UILogic::pressHandler_d100);
	
	ClassDB::bind_method(D_METHOD("pressHandler_roll"), &UILogic::pressHandler_roll);
	
	ClassDB::bind_method(D_METHOD("toggleDropHighest", "p_pressed"), &UILogic::toggleDropHighest);
	ClassDB::bind_method(D_METHOD("toggleDropLowest", "p_pressed"), &UILogic::toggleDropLowest);
	ClassDB::bind_method(D_METHOD("toggleTakeHighest", "p_pressed"), &UILogic::toggleTakeHighest);
	ClassDB::bind_method(D_METHOD("toggleTakeLowest", "p_pressed"), &UILogic::toggleTakeLowest);
	ClassDB::bind_method(D_METHOD("toggleTotal", "p_pressed"), &UILogic::toggleTotal);
	ClassDB::bind_method(D_METHOD("toggleValues", "p_pressed"), &UILogic::toggleValues);
	
	ADD_SIGNAL(MethodInfo("ClearDice"));
	ADD_SIGNAL(MethodInfo("UpdateEquation", PropertyInfo(Variant::DICTIONARY, "diceRoles")));
	ADD_SIGNAL(MethodInfo("SpawnDice", PropertyInfo(Variant::INT, "sides"), PropertyInfo(Variant::INT, "quantity")));
	
	ADD_SIGNAL(MethodInfo("SetVisibility_DropHighest", PropertyInfo(Variant::BOOL, "pressed")));
	ADD_SIGNAL(MethodInfo("SetVisibility_DropLowest", PropertyInfo(Variant::BOOL, "pressed")));
	ADD_SIGNAL(MethodInfo("SetVisibility_TakeHighest", PropertyInfo(Variant::BOOL, "pressed")));
	ADD_SIGNAL(MethodInfo("SetVisibility_TakeLowest", PropertyInfo(Variant::BOOL, "pressed")));
	ADD_SIGNAL(MethodInfo("SetVisibility_Total", PropertyInfo(Variant::BOOL, "pressed")));
	ADD_SIGNAL(MethodInfo("SetVisibility_Values", PropertyInfo(Variant::BOOL, "pressed")));
}

void UILogic::_ready()
{
	isEditor = Engine::get_singleton()->is_editor_hint();
	
	if(!isEditor)
	{
		get_node<Button>("%DiceButtons/%d4")->connect("gui_input", Callable(this, "pressHandler_d4"));
		get_node<Button>("%DiceButtons/%d6")->connect("gui_input", Callable(this, "pressHandler_d6"));
		get_node<Button>("%DiceButtons/%d8")->connect("gui_input", Callable(this, "pressHandler_d8"));
		get_node<Button>("%DiceButtons/%d10")->connect("gui_input", Callable(this, "pressHandler_d10"));
		get_node<Button>("%DiceButtons/%d12")->connect("gui_input", Callable(this, "pressHandler_d12"));
		get_node<Button>("%DiceButtons/%d20")->connect("gui_input", Callable(this, "pressHandler_d20"));
		get_node<Button>("%DiceButtons/%d100")->connect("gui_input", Callable(this, "pressHandler_d100"));
		
		get_node<Button>("%Roll")->connect("pressed", Callable(this, "pressHandler_roll"));
		
		get_node<CheckButton>("%DropHighestButton")->connect("toggled", Callable(this, "toggleDropHighest"));
		get_node<CheckButton>("%DropLowestButton")->connect("toggled", Callable(this, "toggleDropLowest"));
		get_node<CheckButton>("%TakeHighestButton")->connect("toggled", Callable(this, "toggleTakeHighest"));
		get_node<CheckButton>("%TakeLowestButton")->connect("toggled", Callable(this, "toggleTakeLowest"));
		get_node<CheckButton>("%TotalButton")->connect("toggled", Callable(this, "toggleTotal"));
		get_node<CheckButton>("%ValuesButton")->connect("toggled", Callable(this, "toggleValues"));
	}
}

void UILogic::dicePressHandler(const int sides, const Ref<InputEvent> event)
{
	if(event.is_valid() && event->is_class("InputEventMouseButton"))
	{
		auto e = (Ref<InputEventMouseButton>)event;
		if(e->is_pressed())
			updateEquation(sides, e->get_button_index() == MOUSE_BUTTON_RIGHT);
	}
}

void UILogic::pressHandler_d4(const Ref<InputEvent> event) { dicePressHandler(DiceType::Four, event); }
void UILogic::pressHandler_d6(const Ref<InputEvent> event) { dicePressHandler(DiceType::Six, event); }
void UILogic::pressHandler_d8(const Ref<InputEvent> event) { dicePressHandler(DiceType::Eight, event); }
void UILogic::pressHandler_d10(const Ref<InputEvent> event) { dicePressHandler(DiceType::Ten, event); }
void UILogic::pressHandler_d12(const Ref<InputEvent> event) { dicePressHandler(DiceType::Twelve, event); }
void UILogic::pressHandler_d20(const Ref<InputEvent> event) { dicePressHandler(DiceType::Twenty, event); }
void UILogic::pressHandler_d100(const Ref<InputEvent> event) { dicePressHandler(DiceType::Hundred, event); }

void UILogic::pressHandler_roll() { spawnDice(); }

void UILogic::toggleDropHighest(const bool pressed) { emit_signal("SetVisibility_DropHighest", pressed); }
void UILogic::toggleDropLowest(const bool pressed) { emit_signal("SetVisibility_DropLowest", pressed); }
void UILogic::toggleTakeHighest(const bool pressed) { emit_signal("SetVisibility_TakeHighest", pressed); }
void UILogic::toggleTakeLowest(const bool pressed) { emit_signal("SetVisibility_TakeLowest", pressed); }
void UILogic::toggleTotal(const bool pressed) { emit_signal("SetVisibility_Total", pressed); }
void UILogic::toggleValues(const bool pressed) { emit_signal("SetVisibility_Values", pressed); }

void UILogic::spawnDice()
{
	emit_signal("ClearDice");
	
	UtilityFunctions::print("Spawning dice!");
	for(auto pair : diceCounts)
	{
		emit_signal("SpawnDice", pair.first, pair.second);
	}
	
	rolledSinceCountUpdate = true;
}

void UILogic::updateEquation(const int sides, const bool reduce)
{
	if(rolledSinceCountUpdate)
	{
		diceCounts.clear();
		rolledSinceCountUpdate = false;
	}
	
	if(reduce)
		diceCounts[sides]--;
	else
		diceCounts[sides]++;
	
	if(diceCounts[sides] < 1)
		diceCounts.erase(sides);
	
	Dictionary dict;
	for(auto pair : diceCounts)
	{
		dict[pair.first] = pair.second;
	}
	
	emit_signal("UpdateEquation", dict);
}
