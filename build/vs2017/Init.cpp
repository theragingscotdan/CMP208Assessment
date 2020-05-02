#include "Init.h"
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>


Initial::Initial()
{

}

//void Initial::InitPlayer(PrimitiveBuilder* primBuild, b2World* world, Player* player_, b2Body* player_body_)
//{
//	// setup the mesh for the player
//	player_->set_mesh(primBuild->GetDefaultCubeMesh());
//
//	// create a physics body for the player
//	b2BodyDef player_body_def;
//	player_body_def.type = b2_dynamicBody;
//	player_body_def.position = b2Vec2(0.0f, 4.0f);
//
//	player_body_ = world->CreateBody(&player_body_def);
//
//	// create the shape for the player
//	b2PolygonShape player_shape;
//	player_shape.SetAsBox(0.5f, 0.5f);
//
//	// create the fixture
//	b2FixtureDef player_fixture_def;
//	player_fixture_def.shape = &player_shape;
//	player_fixture_def.density = 1.0f;
//
//	// create the fixture on the rigid body
//	player_body_->CreateFixture(&player_fixture_def);
//
//	// update visuals from simulation data
//	player_->UpdateFromSimulation(player_body_);
//
//	// create a connection between the rigid body and GameObject
//	player_body_->SetUserData(&player_);
//}

void Initial::InitGround(PrimitiveBuilder* primBuild, b2World* world, gef::Mesh* ground_mesh_, GameObject* ground_, b2Body* ground_body_)
{
	// ground dimensions
	gef::Vector4 ground_half_dimensions(5.0f, 0.5f, 0.5f);

	// setup the mesh for the ground
	ground_mesh_ = primBuild->CreateBoxMesh(ground_half_dimensions);
	ground_->set_mesh(ground_mesh_);

	// create a physics body
	b2BodyDef body_def;
	body_def.type = b2_staticBody;
	body_def.position = b2Vec2(0.0f, 0.0f);

	ground_body_ = world->CreateBody(&body_def);

	// create the shape
	b2PolygonShape shape;
	shape.SetAsBox(ground_half_dimensions.x(), ground_half_dimensions.y());

	// create the fixture
	b2FixtureDef fixture_def;
	fixture_def.shape = &shape;

	// create the fixture on the rigid body
	ground_body_->CreateFixture(&fixture_def);

	// update visuals from simulation data
	ground_->UpdateFromSimulation(ground_body_);
}