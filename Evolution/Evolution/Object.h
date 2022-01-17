#ifndef OBJECT_H
#define OBJECT_H
#include <iostream>
#include <SFML/Graphics.hpp>

class Object
{
public:
	sf::Color color;
	Object(sf::Color aColor = sf::Color::Black);
	virtual ~Object();
};

#endif // !OBJECT_H