#pragma once
#include <box2d/box2d.h>

#include "game_object.h"
#include "primitive_builder.h"

class Collectable :
	public GameObject
{
public:
	Collectable();
	~Collectable();
	void InitCollectable(PrimitiveBuilder* primBuild, b2World* world, float x, float y);

private:
	bool isPickedUp;
	b2Body* collect_body_;
};

