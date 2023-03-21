#ifndef DICEEQUATION_HH
#define DICEEQUATION_HH

#include <godot_cpp/classes/text_edit.hpp>
#include <map>

namespace godot
{
	class DiceEquation : public TextEdit
	{
		GDCLASS(DiceEquation, TextEdit)
		
		public:
			void _ready() override;
			void updateText(const Dictionary diceCounts);
		
		protected:
			static void _bind_methods();
		
		private:
			bool isEditor;
			String dieFormat = "{0}d{1}";
			
			DiceEquation() = default;
			~DiceEquation() = default;
	};
}

#endif //DICEEQUATION_HH
