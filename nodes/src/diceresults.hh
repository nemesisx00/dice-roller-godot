#ifndef DICERESULTS_HH
#define DICERESULTS_HH

#include <godot_cpp/classes/text_edit.hpp>
#include <godot_cpp/classes/v_box_container.hpp>

namespace godot
{
	class DiceResults : public VBoxContainer
	{
		GDCLASS(DiceResults, VBoxContainer)
		
		public:
			void _ready() override;
			void update(const TypedArray<Dictionary> results);
			void clearResults();
			
			bool getDropHighest() const { return DropHighest; }
			void setDropHighest(const bool pressed);
			
			bool getDropLowest() const { return DropLowest; }
			void setDropLowest(const bool pressed);
			
			bool getTakeHighest() const { return TakeHighest; }
			void setTakeHighest(const bool pressed);
			
			bool getTakeLowest() const { return TakeLowest; }
			void setTakeLowest(const bool pressed);
			
			bool getTotal() const { return Total; }
			void setTotal(const bool pressed);
			
			bool getValues() const { return Values; }
			void setValues(const bool pressed);
		
		protected:
			static void _bind_methods();
		
		private:
			bool isEditor;
			const String dieFormat = "{0}d{1}";
			
			bool DropHighest = false;
			bool DropLowest = false;
			bool TakeHighest = false;
			bool TakeLowest = false;
			bool Total = true;
			bool Values = false;
			
			DiceResults() = default;
			~DiceResults() = default;
	};
}

#endif //DICERESULTS_HH
