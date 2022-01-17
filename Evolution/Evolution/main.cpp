#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Object.h"
#include "Wall.h"

void draw_map(sf::RenderWindow& window, std::vector<std::vector<Object*>>& map)
{
    sf::CircleShape hexagon(25, 6);
    hexagon.setOutlineColor(sf::Color::Black);
    hexagon.setOutlineThickness(5);
    hexagon.setFillColor(sf::Color::White);
    hexagon.setOrigin(25, 25);
    hexagon.setPosition(25, 25);
    for (int i = 0; i < 1; i++)
        for (int j = 1; j < 10; j++)
        {
            hexagon.setFillColor(map[i][j]->color);
            hexagon.setPosition(50 * i + 25 * (j % 2), 40 * j);
            window.draw(hexagon);
        }
    window.draw(hexagon);
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");
    std::vector<std::vector<Object*>> map(10, std::vector<Object*>(10, new Object()));
    for (int i = 0; i < 10; i++)
    {
        map[i][0] = new Wall();
        map[i][9] = new Wall();
        map[0][i] = new Wall();
        map[9][i] = new Wall();
    }
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        




        window.clear();
        draw_map(window, map);
        window.display();
    }

    return 0;
}