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
			
			void pressHandler_d4();
			void pressHandler_d6();
			void pressHandler_d8();
			void pressHandler_d10();
			void pressHandler_d12();
			void pressHandler_d20();
			void pressHandler_d100();
			
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
			
			UILogic() = default;
			~UILogic() = default;
	};
}

#endif //UILOGIC_HH
