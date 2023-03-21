#ifndef DIENODE_HH
#define DIENODE_HH

#include "dicemanager.hh"
#include <godot_cpp/classes/rigid_body3d.hpp>

namespace godot
{
	enum DiceType
	{
		Four = 4,
		Six = 6, 
		Eight = 8,
		Ten = 10,
		Twelve = 12,
		Twenty = 20,
		Hundred = 100,
	};
	
	class DieNode : public RigidBody3D
	{
		GDCLASS(DieNode, RigidBody3D)
		
		public:
			void _ready() override;
			
			int getValue() const;
			void handleSleepingStateChange();
			
			NodePath getDisplayValue() const { return DisplayValuePath; }
			void setDisplayValue(const NodePath path) { DisplayValuePath = path; }
			
			bool getDisplayValueLabel() const { return DisplayValueLabel; }
			void setDisplayValueLabel(const bool display) { DisplayValueLabel = display; }
			
			bool getGuessNotFlat() const { return GuessNotFlat; }
			void setGuessNotFlat(const bool guess) { GuessNotFlat = guess; }
			
			int getSides() const { return Sides; };
			void setSides(const int sides) { Sides = sides; }
			
			NodePath getSidesPath() const { return SidesPath; }
			void setSidesPath(const NodePath path) { SidesPath = path; }
		
		protected:
			static void _bind_methods();
		
		private:
			bool isEditor;
			
			bool DisplayValueLabel = true;
			bool GuessNotFlat = true;
			int Sides;
			NodePath SidesPath = "%Sides";
			NodePath DisplayValuePath = "%DisplayValue";
			
			DieNode() = default;
			~DieNode() = default;
			void updateDisplayLabel();
	};
}

#endif //DIENODE_HH
