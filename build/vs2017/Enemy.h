#pragma once
#include "game_object.h"
#include "primitive_builder.h"
#include <box2d/box2d.h>

class Enemy :
	public GameObject


{
public:
	Enemy();
	~Enemy();

	void InitEnemy(PrimitiveBuilder* primBuild, b2World* world, b2Vec2 initialPosition);
	void MoveEnemy(float minX, float maxX, float minY, float maxY);
	void PlaceEnemy();

	enum direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT

	};

protected:
	
	

private:
	b2Body* enemy_body_;
	//b2Vec2 initialPosition[5];
	direction dir = DOWN;

	

};

