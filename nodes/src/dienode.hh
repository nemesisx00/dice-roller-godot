#ifndef DIENODE_HH
#define DIENODE_HH

#include <godot_cpp/classes/node3d.hpp>
#include <memory>

namespace godot
{
	class DieNode : public Node3D
	{
		GDCLASS(DieNode, Node3D)
		
		public:
			void _ready() override;
			int getValue() const;
			
		protected:
			static void _bind_methods();
		
		private:
			DieNode() = default;
			~DieNode() = default;
	};
}

#endif //DIENODE_HH
