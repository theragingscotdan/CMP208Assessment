#include "Enemy.h"



Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

void Enemy::InitEnemy(PrimitiveBuilder* primBuild, b2World* world, b2Vec2 initialPosition)
{
	// setup the mesh for the player
	set_mesh(primBuild->GetDefaultCubeMesh());
	SetScale(gef::Vector4(0.5, 0.5, 0.5));

	// create a physics body for the player

	b2BodyDef enemy_body_def;
	enemy_body_def.type = b2_staticBody;
	//PlaceEnemy();
	//for (int j = 0; j < 5; ++j)
	//{
	//	enemy_body_def.position = b2Vec2(initialPosition[j].x, initialPosition[j].y);

	//}
	enemy_body_def.position = b2Vec2(initialPosition.x, initialPosition.y);
	enemy_body_ = world->CreateBody(&enemy_body_def);

	// create the shape for the player
	b2PolygonShape enemy_shape;
	enemy_shape.SetAsBox(0.25f, 0.25f);

	// create the fixture
	b2FixtureDef enemy_fixture_def;
	enemy_fixture_def.shape = &enemy_shape;
	enemy_fixture_def.density = 1.0f;

	// create the fixture on the rigid body
	enemy_body_->CreateFixture(&enemy_fixture_def);

	set_type(ENEMY);
	// update visuals from simulation data
	UpdateFromSimulation(enemy_body_);


	// create a connection between the rigid body and GameObject
	enemy_body_->SetUserData(this);
}

void Enemy::MoveEnemy(float minX, float maxX, float minY, float maxY)
{
	if (enemy_body_)
	{
		//initialPosition. b2Vec2(0.0, 1.0);
		// consider slight randomisation but always move in a rough square
		// eg up 5, along 4, down4, right 6
		
		float xrange = rand() % (int)maxX + (int)minX;
		float yrange = rand() % (int)maxY + (int)minY;


		switch (dir)
		{
			case UP:
			{
				enemy_body_->ApplyLinearImpulseToCenter(b2Vec2(0, yrange), true);
				break;
			}
			case DOWN:
			{
				enemy_body_->ApplyLinearImpulseToCenter(b2Vec2(0, -yrange), true);
				break;
			}
			case LEFT:
			{
				enemy_body_->ApplyLinearImpulseToCenter(b2Vec2(-xrange, 0), true);
				break;
			}
			case RIGHT:
			{
				enemy_body_->ApplyLinearImpulseToCenter(b2Vec2(xrange, 0), true);
				break;
			}
		}

	}
}

//void Enemy::PlaceEnemy()
//{
//	initialPosition[1] = (b2Vec2(5.0f, 4.0f));
//	initialPosition[2] = (b2Vec2(1.0f, 12.0f));
//}
