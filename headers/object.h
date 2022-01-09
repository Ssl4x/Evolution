#ifndef OBJECT_H
#define OBJECT_H

//--------------------------------------------------------------------------------

class Object
{
public:
	enum ObjectType 
	{ 
		NUN		=	0, 
		VOID	=	1, 
		BOT		=	2, 
		FOOD	=	3, 
		POISON	=	4, 
		WALL	=	5
	};

	Object(ObjectType aType = NUN);
	virtual ~Object();

	ObjectType getType() const;

private:
	ObjectType mType;
};

//--------------------------------------------------------------------------------

#endif // OBJECT_H