#ifndef HELLOCPP_HH
#define HELLOCPP_HH

#include <godot_cpp/classes/label.hpp>

namespace godot
{
	class HelloCpp : public Label
	{
		GDCLASS(HelloCpp, Label)
		
		public:
			void _ready() override;
		
		protected:
			static void _bind_methods();
		
		private:
			String initialText;
			
			HelloCpp();
			~HelloCpp() = default;
	};
}

#endif //HELLOCPP_HH
