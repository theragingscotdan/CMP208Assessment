#pragma once

#include <box2d/Box2D.h>
//#include <graphics/mesh_instance.h>
#include "game_object.h"
#include "primitive_builder.h"

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class InputManager;
	class Renderer3D;
}

class Initial
{
public:
	Initial();
	void InitPlayer(PrimitiveBuilder* primBuild, b2World* world, Player* player_, b2Body* player_body_);
	void InitGround(PrimitiveBuilder* primBuild, b2World* world, gef::Mesh* ground_mesh_, GameObject* ground_, b2Body* ground_body_);


protected:

	


private:
	
	//PrimitiveBuilder* primitive_builder_;

	//// create the physics world
	//b2World* world_;

	// player variables
	/*Player* player_;
	b2Body* player_body_;*/

	//// ground variables
	//gef::Mesh* ground_mesh_;
	//GameObject ground_;
	//b2Body* ground_body_;
};