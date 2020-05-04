#include "game_object.h"
#include <system/debug_log.h>

//
// UpdateFromSimulation
// 
// Update the transform of this object from a physics rigid body
//
void GameObject::UpdateFromSimulation(const b2Body* body)
{
	if (body)
	{
		// setup object rotation
		gef::Matrix44 object_rotation;
		gef::Matrix44 scale;
		object_rotation.RotationZ(body->GetAngle());

		// setup the object translation
		gef::Vector4 object_translation(body->GetPosition().x, body->GetPosition().y, 0.0f);
		scale.Scale(scale_);

		// build object transformation matrix
		gef::Matrix44 object_transform = object_rotation * scale;
		object_transform.SetTranslation(object_translation);
		set_transform(object_transform);
	}
}

void GameObject::MyCollisionResponse()
{
	//gef::DebugOut("A collision has happened.\n");
}

void GameObject::SetScale(gef::Vector4 _scale)
{
	scale_ = _scale;
}

OBJECT_TYPE GameObject::GetType()
{
	return OBJECT_TYPE();
}

//Player::Player()
//{
//	set_type(PLAYER);
//}
//
//void Player::DecrementHealth()
//{
//	//gef::DebugOut("Player has taken damage.\n");
//}


