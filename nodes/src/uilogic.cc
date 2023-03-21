#include <godot_cpp/classes/global_constants.hpp>
#include <uilogic.hh>

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
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
	
	ClassDB::bind_method(D_METHOD("pressHandler_average"), &UILogic::pressHandler_average);
	ClassDB::bind_method(D_METHOD("pressHandler_dropHighest"), &UILogic::pressHandler_dropHighest);
	ClassDB::bind_method(D_METHOD("pressHandler_dropLowest"), &UILogic::pressHandler_dropLowest);
	ClassDB::bind_method(D_METHOD("pressHandler_takeHighest"), &UILogic::pressHandler_takeHighest);
	ClassDB::bind_method(D_METHOD("pressHandler_takeLowest"), &UILogic::pressHandler_takeLowest);
	ClassDB::bind_method(D_METHOD("pressHandler_total"), &UILogic::pressHandler_total);
	
	ADD_SIGNAL(MethodInfo("ClearDice"));
	ADD_SIGNAL(MethodInfo("UpdateEquation", PropertyInfo(Variant::DICTIONARY, "diceRoles")));
	ADD_SIGNAL(MethodInfo("SpawnDice", PropertyInfo(Variant::INT, "sides"), PropertyInfo(Variant::INT, "quantity")));
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
		
		get_node<Button>("%RollButtons/%Average")->connect("pressed", Callable(this, "pressHandler_average"));
		get_node<Button>("%RollButtons/%DropHighest")->connect("pressed", Callable(this, "pressHandler_dropHighest"));
		get_node<Button>("%RollButtons/%DropLowest")->connect("pressed", Callable(this, "pressHandler_dropLowest"));
		get_node<Button>("%RollButtons/%TakeHighest")->connect("pressed", Callable(this, "pressHandler_takeHighest"));
		get_node<Button>("%RollButtons/%TakeLowest")->connect("pressed", Callable(this, "pressHandler_takeLowest"));
		get_node<Button>("%RollButtons/%Total")->connect("pressed", Callable(this, "pressHandler_total"));
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

void UILogic::pressHandler_average() { spawnDice(); }
void UILogic::pressHandler_dropHighest() { spawnDice(); }
void UILogic::pressHandler_dropLowest() { spawnDice(); }
void UILogic::pressHandler_takeHighest() { spawnDice(); }
void UILogic::pressHandler_takeLowest() { spawnDice(); }
void UILogic::pressHandler_total() { spawnDice(); }

void UILogic::spawnDice()
{
	emit_signal("ClearDice");
	
	UtilityFunctions::print("Spawning dice!");
	for(auto pair : diceCounts)
	{
		emit_signal("SpawnDice", pair.first, pair.second);
	}
	
	diceCounts.clear();
}

void UILogic::updateEquation(const int sides, const bool reduce)
{
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
