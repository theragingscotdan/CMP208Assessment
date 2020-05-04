#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <graphics/mesh_instance.h>
#include <box2d/Box2D.h>

enum OBJECT_TYPE
{
	PLAYER,
	ENEMY,
	BULLET,
	COLLECTABLE
};

class GameObject : public gef::MeshInstance
{
public:
	void UpdateFromSimulation(const b2Body* body);
	void MyCollisionResponse();

	inline void set_type(OBJECT_TYPE type) { type_ = type; }
	void SetScale(gef::Vector4 _scale);

	inline OBJECT_TYPE type() { return type_; }
	OBJECT_TYPE GetType();
private:
	OBJECT_TYPE type_;
	gef::Vector4 scale_;
};

//class Player : public GameObject
//{
//public:
//	Player();
//	void DecrementHealth();
//};

#endif // _GAME_OBJECT_H