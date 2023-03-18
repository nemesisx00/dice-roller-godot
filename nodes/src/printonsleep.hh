#ifndef PRINTONSLEEP_HH
#define PRINTONSLEEP_HH

#include <godot_cpp/classes/rigid_body3d.hpp>

namespace godot
{
	class PrintOnSleep : public RigidBody3D
	{
		GDCLASS(PrintOnSleep, RigidBody3D)
		
		public:
			void _ready() override;
			void _physics_process(double delta) override;
			
		protected:
			static void _bind_methods();
		
		private:
			bool hasPrinted;
			int current;
			int limit;
			
			PrintOnSleep();
			~PrintOnSleep() = default;
	};
}

#endif //PRINTONSLEEP_HH
