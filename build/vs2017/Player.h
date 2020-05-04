#pragma once

#include <box2d/box2d.h>

#include "game_object.h"
#include "primitive_builder.h"

//enum PlayerState
//{
//	JUMP,
//	ATTACK
//};


class Player : public GameObject
{
public:
	Player();
	void InitPlayer(PrimitiveBuilder* primBuild, b2World* world);
	void Update(float frame_time);

	enum PlayerState
	{
		JUMP,
		ATTACK
	};

	void SetState(PlayerState state);
	b2Body* GetBody();
	void SetScore(int score_);
	int GetScore();
	

private:
	b2Body* player_body_;
	
	PlayerState state_;
	bool canJump = false;
	int m_score = 0;
};

