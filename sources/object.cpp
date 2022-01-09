#include "object.h"

//--------------------------------------------------------------------------------

Object::Object(ObjectType aType) :
	mType (aType)
{}

Object::~Object() {}

Object::ObjectType 
Object::getType() const
{
	return mType;
}

//--------------------------------------------------------------------------------