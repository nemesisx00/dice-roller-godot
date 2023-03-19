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

void UILogic::pressHandler_d4()
{
	diceCounts[DiceType::Four]++;
	UtilityFunctions::print(diceCounts[DiceType::Four], "d", DiceType::Four);
}

void UILogic::pressHandler_d6()
{
	diceCounts[DiceType::Six]++;
	UtilityFunctions::print(diceCounts[DiceType::Six], "d", DiceType::Six);
}

void UILogic::pressHandler_d8()
{
	diceCounts[DiceType::Eight]++;
	UtilityFunctions::print(diceCounts[DiceType::Eight], "d", DiceType::Eight);
}

void UILogic::pressHandler_d10()
{
	diceCounts[DiceType::Ten]++;
	UtilityFunctions::print(diceCounts[DiceType::Ten], "d", DiceType::Ten);
}

void UILogic::pressHandler_d12()
{
	diceCounts[DiceType::Twelve]++;
	UtilityFunctions::print(diceCounts[DiceType::Twelve], "d", DiceType::Twelve);
}

void UILogic::pressHandler_d20()
{
	diceCounts[DiceType::Twenty]++;
	UtilityFunctions::print(diceCounts[DiceType::Twenty], "d", DiceType::Twenty);
}

void UILogic::pressHandler_d100()
{
	diceCounts[DiceType::Hundred]++;
	UtilityFunctions::print(diceCounts[DiceType::Hundred], "d", DiceType::Hundred);
}

void UILogic::pressHandler_average()
{
	UtilityFunctions::print("average");
	spawnDice();
}

void UILogic::pressHandler_dropHighest()
{
	UtilityFunctions::print("drop highest");
	spawnDice();
}

void UILogic::pressHandler_dropLowest()
{
	UtilityFunctions::print("drop lowest");
	spawnDice();
}

void UILogic::pressHandler_takeHighest()
{
	UtilityFunctions::print("take highest");
	spawnDice();
}

void UILogic::pressHandler_takeLowest()
{
	UtilityFunctions::print("take lowest");
	spawnDice();
}

void UILogic::pressHandler_total()
{
	UtilityFunctions::print("total");
	spawnDice();
}

void UILogic::spawnDice()
{
	emit_signal("clear_dice");
	
	UtilityFunctions::print("Spawning dice!");
	for(std::map<int, int>::iterator it=diceCounts.begin(); it != diceCounts.end(); ++it)
	{
		UtilityFunctions::print("spawn: ", it->second, "d", it->first);
		emit_signal("spawn_dice", it->first, it->second);
	}
	
	diceCounts.clear();
}
