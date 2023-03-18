#ifndef DIENODE_HH
#define DIENODE_HH

#include "dicemanager.hh"
#include <godot_cpp/classes/rigid_body3d.hpp>

namespace godot
{
	enum Dice
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
			
			int getSides() const { return Sides; };
			void setSides(const int sides) { Sides = sides; }
		
		protected:
			static void _bind_methods();
		
		private:
			bool isEditor;
			
			int Sides;
			
			DieNode() = default;
			~DieNode() = default;
	};
}

#endif //DIENODE_HH
