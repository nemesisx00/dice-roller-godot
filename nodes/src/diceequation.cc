#include <diceequation.hh>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/dictionary.hpp>

#include <uilogic.hh>

using namespace godot;

void DiceEquation::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("updateText", "p_diceCounts"), &DiceEquation::updateText);
}

void DiceEquation::_ready()
{
	isEditor = Engine::get_singleton()->is_editor_hint();
	
	if(!isEditor)
	{
		auto logic = get_parent()->get_parent()->get_node<UILogic>("UILogic");
		//logic->connect("ClearDice", Callable(this, "clear"));
		logic->connect("UpdateEquation", Callable(this, "updateText"));
	}
}

void DiceEquation::updateText(const Dictionary diceCounts)
{
	String newText;
	
	auto keys = diceCounts.keys();
	auto keySize = keys.size();
	for(int64_t i = 0; i < keySize; i++)
	{
		int key = keys[i];
		
		Array params;
		params.push_back(diceCounts[key]);
		params.push_back(key);
		
		if(newText.length() > 0)
			newText += " + ";
		newText += dieFormat.format(params);
	}
	
	set_text(newText);
}
