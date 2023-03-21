#ifndef UILOGIC_HH
#define UILOGIC_HH

#include <godot_cpp/classes/control.hpp>
#include <map>

namespace godot
{
	class UILogic : public Control
	{
		GDCLASS(UILogic, Control)
		
		public:
			void _ready() override;
			
			void pressHandler_d4(const Ref<InputEvent> event);
			void pressHandler_d6(const Ref<InputEvent> event);
			void pressHandler_d8(const Ref<InputEvent> event);
			void pressHandler_d10(const Ref<InputEvent> event);
			void pressHandler_d12(const Ref<InputEvent> event);
			void pressHandler_d20(const Ref<InputEvent> event);
			void pressHandler_d100(const Ref<InputEvent> event);
			
			void pressHandler_average();
			void pressHandler_dropHighest();
			void pressHandler_dropLowest();
			void pressHandler_takeHighest();
			void pressHandler_takeLowest();
			void pressHandler_total();
		
		protected:
			static void _bind_methods();
		
		private:
			bool isEditor;
			std::map<int, int> diceCounts;
			
			UILogic() = default;
			~UILogic() = default;
			void dicePressHandler(const int sides, const Ref<InputEvent> event);
			void updateEquation(const int sides, const bool reduce);
			void spawnDice();
	};
}

#endif //UILOGIC_HH
