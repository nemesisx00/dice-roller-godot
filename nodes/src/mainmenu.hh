#ifndef MAINMENU_HH
#define MAINMENU_HH

#include <godot_cpp/classes/control.hpp>

namespace godot
{
	class MainMenu : public Control
	{
		GDCLASS(MainMenu, Control)
		
		public:
			void _ready() override;
			
			void pressHandler_godotLicense();
			void pressHandler_quit();
			void pressHandler_start();
		
		protected:
			static void _bind_methods();
		
		private:
			bool isEditor;
			
			MainMenu() = default;
			~MainMenu() = default;
			void quit();
			void start();
	};
}

#endif //MAINMENU_HH
