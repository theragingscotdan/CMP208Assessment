#include "Collectable.h"




Collectable::Collectable()
{
}


Collectable::~Collectable()
{
}

void Collectable::InitCollectable(PrimitiveBuilder* primBuild, b2World* world, float x, float y)
{
	//set_mesh(primBuild->GetDefaultSphereMesh());
	SetScale(gef::Vector4(0.25, 0.25, 0.25));

	// create a physics body for the player
	b2BodyDef collect_body_def;
	collect_body_def.type = b2_staticBody;
	collect_body_def.position = b2Vec2(x, y);

	collect_body_ = world->CreateBody(&collect_body_def);

	// create the shape for the player
	b2PolygonShape collect_shape;
	collect_shape.SetAsBox(0.45f, 0.125f);


	// create the fixture
	b2FixtureDef collect_fixture_def;
	collect_fixture_def.shape = &collect_shape;
	collect_fixture_def.density = 1.0f;

	// create the fixture on the rigid body
	collect_body_->CreateFixture(&collect_fixture_def);

	// update visuals from simulation data
	UpdateFromSimulation(collect_body_);

	set_type(COLLECTABLE);
	isPickedUp = false;
	// create a connection between the rigid body and GameObject
	collect_body_->SetUserData(this);

	
}

b2Body * Collectable::GetBody()
{
	return collect_body_;
}

void Collectable::SetPickUp(bool pickup)
{
	isPickedUp = pickup;
}

bool Collectable::GetPickUp()
{
	return isPickedUp;
}


