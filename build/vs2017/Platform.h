#pragma once

#include <box2d/box2d.h>

#include "game_object.h"
#include "primitive_builder.h"


class platforms : public GameObject
{
public:
	platforms();
	void InitPlatforms(PrimitiveBuilder* primBuild, b2World* world, float x, float y);

protected:

private:
	gef::Mesh* platform_mesh_;
	b2Body* platform_body_;


};