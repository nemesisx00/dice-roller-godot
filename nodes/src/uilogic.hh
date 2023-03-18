#ifndef UILOGIC_HH
#define UILOGIC_HH

#include <godot_cpp/classes/control.hpp>

namespace godot
{
	class UILogic : public Control
	{
		GDCLASS(UILogic, Control)
		
		public:
			void _ready() override;
			
			void clickHandler_d4();
			void clickHandler_d6();
			void clickHandler_d8();
			void clickHandler_d10();
			void clickHandler_d12();
			void clickHandler_d20();
			void clickHandler_d100();
			
			void clickHandler_dropHighest();
			void clickHandler_dropLowest();
			void clickHandler_median();
			void clickHandler_takeHighest();
			void clickHandler_takeLowest();
			void clickHandler_total();
		
		protected:
			static void _bind_methods();
		
		private:
			UILogic() = default;
			~UILogic() = default;
	};
}

#endif //UILOGIC_HH
