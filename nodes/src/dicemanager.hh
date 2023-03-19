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
			
			void handleAsleep(const uint64_t id);
			void handleClearDice();
			void handleSpawnDice(const int sides, const int quantity);
			
		protected:
			static void _bind_methods();
		
		private:
			bool isEditor;
			std::list<uint64_t> nodeIds;
			
			DiceManager() = default;
			~DiceManager() = default;
			void processResults();
	};
}

#endif //DICEMANAGER_HH
