#include "Platform.h"

platforms::platforms()
{

}

void platforms::InitPlatforms(PrimitiveBuilder* primBuild, b2World* world, float x, float y)
{
	gef::Vector4 ground_half_dimensions(1.0f, 0.0625f, 0.5f);
	SetScale(gef::Vector4(1.0, 1.0, 1.0));

	// setup the mesh for the ground
	//primBuild->CreateBoxMesh(ground_half_dimensions);
	set_mesh(primBuild->CreateBoxMesh(ground_half_dimensions));

	// create a physics body
	b2BodyDef body_def;
	body_def.type = b2_staticBody;
	body_def.position = b2Vec2(x, y);

	platform_body_ = world->CreateBody(&body_def);

	// create the shape
	b2PolygonShape shape;
	shape.SetAsBox(ground_half_dimensions.x(), ground_half_dimensions.y());

	// create the fixture
	b2FixtureDef fixture_def;
	fixture_def.shape = &shape;

	// create the fixture on the rigid body
	platform_body_->CreateFixture(&fixture_def);

	platform_body_->SetUserData(this);

	// update visuals from simulation data
	UpdateFromSimulation(platform_body_);
}

void platforms::MovingPlatforms()
{
	// this was inspired and adapted from https://rotatingcanvas.com/moving-platforms-in-box2d/

	if (m_canMove)
	{

	}

}
