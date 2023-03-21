#ifndef DIRECTION_HH
#define DIRECTION_HH

#include <godot_cpp/classes/node3d.hpp>

namespace godot
{
	class Direction : public Node3D
	{
		GDCLASS(Direction, Node3D)
		
		public:
			Vector3 getDirection() const;
			Vector3 getDirectionUnit() const;
			
			Vector3 getEndpoint() const { return Endpoint; }
			void setEndpoint(const Vector3 point) { Endpoint = point; }
			
			int getValue() const { return Value; }
			void setValue(const int value) { Value = value; }
		
		protected:
			static void _bind_methods();
		
		private:
			Vector3 Endpoint;
			int Value = 0;
			
			Direction() = default;
			~Direction() = default;
	};
}

#endif //DIRECTION_HH
