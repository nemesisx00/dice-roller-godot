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
			
			bool getDisplayValueLabel() const { return DisplayValueLabel; }
			void setDisplayValueLabel(const bool display) { DisplayValueLabel = display; }
			
			float_t getDownforceMax() const { return DownforceMax; }
			void setDownforceMax(const float_t max) { DownforceMax = max; }
			
			float_t getDownforceMin() const { return DownforceMin; }
			void setDownforceMin(const float_t min) { DownforceMin = min; }
			
			float_t getForceMax() const { return ForceMax; }
			void setForceMax(const float_t max) { ForceMax = max; }
			
			float_t getForceMin() const { return ForceMin; }
			void setForceMin(const float_t min) { ForceMin = min; }
			
			bool getGuessNotFlat() const { return GuessNotFlat; }
			void setGuessNotFlat(const bool guess) { GuessNotFlat = guess; }
			
			bool getGroupImpulse() const { return GroupImpulse; }
			void setGroupImpulse(const bool group) { GroupImpulse = group; }
			
			float_t getPositionMax() const { return PositionMax; }
			void setPositionMax(const float_t max) { PositionMax = max; }
			
			float_t getPositionMin() const { return PositionMin; }
			void setPositionMin(const float_t min) { PositionMin = min; }
			
			float_t getRotationMax() const { return RotationMax; }
			void setRotationMax(const float_t max) { RotationMax = max; }
			
			float_t getRotationMin() const { return RotationMin; }
			void setRotationMin(const float_t min) { RotationMin = min; }
			
			float_t getSpawnHeight() const { return SpawnHeight; }
			void setSpawnHeight(const float_t height) { SpawnHeight = height; }
			
		protected:
			static void _bind_methods();
		
		private:
			bool isEditor;
			std::list<uint64_t> nodeIds;
			
			bool DisplayValueLabel = true;
			float_t DownforceMax = -1;
			float_t DownforceMin = -5;
			float_t ForceMax = 50;
			float_t ForceMin = -50;
			bool GuessNotFlat = true;
			bool GroupImpulse = true;
			float_t PositionMax = 15;
			float_t PositionMin = -15;
			float_t RotationMax = 3;
			float_t RotationMin = -3;
			float_t SpawnHeight = 15;
			
			DiceManager() = default;
			~DiceManager() = default;
			void processResults();
	};
}

#endif //DICEMANAGER_HH
