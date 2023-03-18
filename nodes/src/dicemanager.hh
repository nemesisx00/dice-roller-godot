#ifndef DICEMANAGER_HH
#define DICEMANAGER_HH

#include <godot_cpp/classes/node3d.hpp>

namespace godot
{
	class DiceManager : public Node3D
	{
		GDCLASS(DiceManager, Node3D)
		
		public:
			void _ready() override;
			
			void handleSpawnDie(const int sides, const int quantity);
			
		protected:
			static void _bind_methods();
		
		private:
			bool isEditor;
			
			DiceManager() = default;
			~DiceManager() = default;
	};
}

#endif //DICEMANAGER_HH
