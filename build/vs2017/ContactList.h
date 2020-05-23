#pragma once

#include <box2d/box2d.h>
// this class was 


class ContactList
{
public:
	ContactList();
	~ContactList();

	class MyContactListener : public b2ContactListener
	{
		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);

private: 
	int numContacts = 0;
};

