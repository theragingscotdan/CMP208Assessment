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
	void AddScore(int score_);
	int GetScore();
	int GetLives();
	int GetHealth();
	void ReduceHealth();// (int health_);
	void LoseLife();
	int GetJumping();

private:
	b2Body* player_body_;
	
	PlayerState state_;
	bool canJump = false;
	int m_score = 0;
	int m_lives = 3;
	int m_health = 3;
	int m_jumpEnd = 0;
};

