#include <uilogic.hh>

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
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
	
	ADD_SIGNAL(MethodInfo("clear_dice"));
	ADD_SIGNAL(MethodInfo("spawn_dice", PropertyInfo(Variant::INT, "sides"), PropertyInfo(Variant::INT, "quantity")));
}

void UILogic::_ready()
{
	isEditor = Engine::get_singleton()->is_editor_hint();
	
	if(!isEditor)
	{
		get_node<Button>("%DiceButtons/%d4")->connect("pressed", Callable(this, "pressHandler_d4"));
		get_node<Button>("%DiceButtons/%d6")->connect("pressed", Callable(this, "pressHandler_d6"));
		get_node<Button>("%DiceButtons/%d8")->connect("pressed", Callable(this, "pressHandler_d8"));
		get_node<Button>("%DiceButtons/%d10")->connect("pressed", Callable(this, "pressHandler_d10"));
		get_node<Button>("%DiceButtons/%d12")->connect("pressed", Callable(this, "pressHandler_d12"));
		get_node<Button>("%DiceButtons/%d20")->connect("pressed", Callable(this, "pressHandler_d20"));
		get_node<Button>("%DiceButtons/%d100")->connect("pressed", Callable(this, "pressHandler_d100"));
		
		get_node<Button>("%RollButtons/%Average")->connect("pressed", Callable(this, "pressHandler_average"));
		get_node<Button>("%RollButtons/%DropHighest")->connect("pressed", Callable(this, "pressHandler_dropHighest"));
		get_node<Button>("%RollButtons/%DropLowest")->connect("pressed", Callable(this, "pressHandler_dropLowest"));
		get_node<Button>("%RollButtons/%TakeHighest")->connect("pressed", Callable(this, "pressHandler_takeHighest"));
		get_node<Button>("%RollButtons/%TakeLowest")->connect("pressed", Callable(this, "pressHandler_takeLowest"));
		get_node<Button>("%RollButtons/%Total")->connect("pressed", Callable(this, "pressHandler_total"));
	}
}

void UILogic::pressHandler_d4() { diceCounts[DiceType::Four]++; }
void UILogic::pressHandler_d6() { diceCounts[DiceType::Six]++; }
void UILogic::pressHandler_d8() { diceCounts[DiceType::Eight]++; }
void UILogic::pressHandler_d10() { diceCounts[DiceType::Ten]++; }
void UILogic::pressHandler_d12() { diceCounts[DiceType::Twelve]++; }
void UILogic::pressHandler_d20() { diceCounts[DiceType::Twenty]++; }
void UILogic::pressHandler_d100() { diceCounts[DiceType::Hundred]++; }

void UILogic::pressHandler_average()
{
	spawnDice();
}

void UILogic::pressHandler_dropHighest()
{
	spawnDice();
}

void UILogic::pressHandler_dropLowest()
{
	spawnDice();
}

void UILogic::pressHandler_takeHighest()
{
	spawnDice();
}

void UILogic::pressHandler_takeLowest()
{
	spawnDice();
}

void UILogic::pressHandler_total()
{
	spawnDice();
}

void UILogic::spawnDice()
{
	emit_signal("clear_dice");
	
	UtilityFunctions::print("Spawning dice!");
	for(auto pair : diceCounts)
	{
		emit_signal("spawn_dice", pair.first, pair.second);
	}
	
	diceCounts.clear();
}
