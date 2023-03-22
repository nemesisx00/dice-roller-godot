#include <diceresults.hh>

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <dicemanager.hh>
#include <rollresult.hh>
#include <uilogic.hh>

using namespace godot;

void DiceResults::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("update", "p_results"), &DiceResults::update);
	ClassDB::bind_method(D_METHOD("clearResults"), &DiceResults::clearResults);
	
	ClassDB::bind_method(D_METHOD("getDropHighest"), &DiceResults::getDropHighest);
	ClassDB::bind_method(D_METHOD("setDropHighest", "p_display"), &DiceResults::setDropHighest);
	ClassDB::add_property("DiceResults", PropertyInfo(Variant::BOOL, "Show Drop Highest"), "setDropHighest", "getDropHighest");
	
	ClassDB::bind_method(D_METHOD("getDropLowest"), &DiceResults::getDropLowest);
	ClassDB::bind_method(D_METHOD("setDropLowest", "p_display"), &DiceResults::setDropLowest);
	ClassDB::add_property("DiceResults", PropertyInfo(Variant::BOOL, "Show Drop Lowest"), "setDropLowest", "getDropLowest");
	
	ClassDB::bind_method(D_METHOD("getTakeHighest"), &DiceResults::getTakeHighest);
	ClassDB::bind_method(D_METHOD("setTakeHighest", "p_display"), &DiceResults::setTakeHighest);
	ClassDB::add_property("DiceResults", PropertyInfo(Variant::BOOL, "Show Take Highest"), "setTakeHighest", "getTakeHighest");
	
	ClassDB::bind_method(D_METHOD("getTakeLowest"), &DiceResults::getTakeLowest);
	ClassDB::bind_method(D_METHOD("setTakeLowest", "p_display"), &DiceResults::setTakeLowest);
	ClassDB::add_property("DiceResults", PropertyInfo(Variant::BOOL, "Show Take Lowest"), "setTakeLowest", "getTakeLowest");
	
	ClassDB::bind_method(D_METHOD("getTotal"), &DiceResults::getTotal);
	ClassDB::bind_method(D_METHOD("setTotal", "p_display"), &DiceResults::setTotal);
	ClassDB::add_property("DiceResults", PropertyInfo(Variant::BOOL, "Show Total"), "setTotal", "getTotal");
	
	ClassDB::bind_method(D_METHOD("getValues"), &DiceResults::getValues);
	ClassDB::bind_method(D_METHOD("setValues", "p_display"), &DiceResults::setValues);
	ClassDB::add_property("DiceResults", PropertyInfo(Variant::BOOL, "Show Rolled Values"), "setValues", "getValues");
}

void DiceResults::_ready()
{
	isEditor = Engine::get_singleton()->is_editor_hint();
	
	if(!isEditor)
	{
		auto diceManager = get_parent()->get_parent()->get_parent()->get_node<DiceManager>("%Dice");
		diceManager->connect("RollCompleted", Callable(this, "update"));
		
		auto logic = get_parent()->get_parent()->get_node<UILogic>("UILogic");
		logic->connect("ClearDice", Callable(this, "clearResults"));
		logic->connect("SetVisibility_DropHighest", Callable(this, "setDropHighest"));
		logic->connect("SetVisibility_DropLowest", Callable(this, "setDropLowest"));
		logic->connect("SetVisibility_TakeHighest", Callable(this, "setTakeHighest"));
		logic->connect("SetVisibility_TakeLowest", Callable(this, "setTakeLowest"));
		logic->connect("SetVisibility_Total", Callable(this, "setTotal"));
		logic->connect("SetVisibility_Values", Callable(this, "setValues"));
	}
}

void DiceResults::clearResults()
{
	String empty;
	get_node<TextEdit>("%Total")->set_text(empty);
	get_node<TextEdit>("%DropHighest")->set_text(empty);
	get_node<TextEdit>("%DropLowest")->set_text(empty);
	get_node<TextEdit>("%TakeHighest")->set_text(empty);
	get_node<TextEdit>("%TakeLowest")->set_text(empty);
	get_node<TextEdit>("%Values")->set_text(empty);
}

void DiceResults::update(const TypedArray<Dictionary> results)
{
	String format("{0}");
	Array params;
	
	int dropHighest = 0;
	int dropLowest = 0;
	int takeHighest = 0;
	int takeLowest = 0;
	int total = 0;
	String valuesString = "";
	
	auto resultsSize = results.size();
	for(int64_t i = 0; i < resultsSize; i++)
	{
		auto result = RollResult(results[i]);
		UtilityFunctions::print(result.values.size(), "d", result.sides, " Total: ", result.total);
		
		String tempValues;
		for(auto val : result.values)
		{
			if(tempValues.length() > 0)
				tempValues += ", ";
			params.clear();
			params.push_back(val);
			tempValues += format.format(params);
		}
		
		if(valuesString.length() > 0)
			valuesString += " + ";
		params.clear();
		params.push_back(result.sides);
		valuesString += "d" + format.format(params) + "[" + tempValues + "]";
		
		if(result.values.size() > 1)
		{
			dropHighest += result.total - result.highest;
			dropLowest += result.total - result.lowest;
		}
		else
		{
			dropHighest += result.total;
			dropLowest += result.total;
		}
		
		takeHighest += result.highest;
		takeLowest += result.lowest;
		total += result.total;
	}
	
	String dropHighestString;
	String dropLowestString;
	String takeHighestString;
	String takeLowestString;
	String totalString;
	
	if(dropHighest > 0)
	{
		params.clear();
		params.push_back(dropHighest);
		dropHighestString = format.format(params);
	}
	
	if(dropLowest > 0)
	{
		params.clear();
		params.push_back(dropLowest);
		dropLowestString = format.format(params);
	}
	
	if(takeHighest > 0)
	{
		params.clear();
		params.push_back(takeHighest);
		takeHighestString = format.format(params);
	}
	
	if(takeLowest > 0)
	{
		params.clear();
		params.push_back(takeLowest);
		takeLowestString = format.format(params);
	}
	
	if(total > 0)
	{
		params.clear();
		params.push_back(total);
		totalString = format.format(params);
	}
	
	get_node<TextEdit>("%Total")->set_text(totalString);
	get_node<TextEdit>("%DropHighest")->set_text(dropHighestString);
	get_node<TextEdit>("%DropLowest")->set_text(dropLowestString);
	get_node<TextEdit>("%TakeHighest")->set_text(takeHighestString);
	get_node<TextEdit>("%TakeLowest")->set_text(takeLowestString);
	get_node<TextEdit>("%Values")->set_text(valuesString);
}

void DiceResults::setDropHighest(const bool pressed)
{
	DropHighest = pressed;
	auto parent = Node::cast_to<HBoxContainer>(get_node<TextEdit>("%DropHighest")->get_parent());
	if(DropHighest)
		parent->show();
	else
		parent->hide();
}

void DiceResults::setDropLowest(const bool pressed)
{
	DropLowest = pressed;
	auto parent = Node::cast_to<HBoxContainer>(get_node<TextEdit>("%DropLowest")->get_parent());
	if(DropLowest)
		parent->show();
	else
		parent->hide();
}

void DiceResults::setTakeHighest(const bool pressed)
{
	TakeHighest = pressed;
	auto parent = Node::cast_to<HBoxContainer>(get_node<TextEdit>("%TakeHighest")->get_parent());
	if(TakeHighest)
		parent->show();
	else
		parent->hide();
}

void DiceResults::setTakeLowest(const bool pressed)
{
	TakeLowest = pressed;
	auto parent = Node::cast_to<HBoxContainer>(get_node<TextEdit>("%TakeLowest")->get_parent());
	if(TakeLowest)
		parent->show();
	else
		parent->hide();
}

void DiceResults::setTotal(const bool pressed)
{
	TakeLowest = pressed;
	auto parent = Node::cast_to<HBoxContainer>(get_node<TextEdit>("%Total")->get_parent());
	if(TakeLowest)
		parent->show();
	else
		parent->hide();
}

void DiceResults::setValues(const bool pressed)
{
	TakeLowest = pressed;
	auto parent = Node::cast_to<VBoxContainer>(get_node<TextEdit>("%Values")->get_parent());
	if(TakeLowest)
		parent->show();
	else
		parent->hide();
}
