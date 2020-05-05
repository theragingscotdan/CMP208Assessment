#include "player.h"

Player::Player() 
{
	

}

void Player::InitPlayer(PrimitiveBuilder* primBuild, b2World* world)
{
	// setup the mesh for the player
	set_mesh(primBuild->GetDefaultCubeMesh());
	SetScale(gef::Vector4(0.5, 0.5, 0.5));

	// create a physics body for the player
	b2BodyDef player_body_def;
	player_body_def.type = b2_dynamicBody;
	player_body_def.position = b2Vec2(0.0f, -3.0f);

	player_body_ = world->CreateBody(&player_body_def);

	// create the shape for the player
	b2PolygonShape player_shape;
	player_shape.SetAsBox(0.25f, 0.25f);

	// create the fixture
	b2FixtureDef player_fixture_def;
	player_fixture_def.shape = &player_shape;
	player_fixture_def.density = 1.0f;

	// create the fixture on the rigid body
	player_body_->CreateFixture(&player_fixture_def);

	set_type(PLAYER);
	// update visuals from simulation data
	UpdateFromSimulation(player_body_);
	

	// create a connection between the rigid body and GameObject
	player_body_->SetUserData(this);


}

void Player::Update(float frame_time)
{
	if (state_ = JUMP)
	{
		canJump = true;
	} 
	else
	{
		canJump = false;
	}
}

void Player::SetState(PlayerState state)
{
	state_ = state;
}

b2Body* Player::GetBody()
{
	return player_body_;
}

void Player::AddScore(int score_)
{
	m_score += score_;
	
}

int Player::GetScore()
{
	return m_score;
}

int Player::GetLives()
{
	return m_lives;
}

