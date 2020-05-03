#pragma once

#include <box2d/box2d.h>

#include "game_object.h"
#include "primitive_builder.h"

enum PlayerState
{
	JUMP,
	ATTACK
};


class Player : public GameObject
{
public:
	Player();
	void InitPlayer(PrimitiveBuilder* primBuild, b2World* world, b2Body* player_body_);
	void Update(float frame_time);
	void SetState(PlayerState state);


private:
	PlayerState state_;
	bool canJump = false;
};

