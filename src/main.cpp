#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <Eigen/Dense>

#include "game_of_life.h"

int main()
{
    int cellXCount = 6;
    int cellYCount = 6;

    auto game = gol::GameOfLife(cellXCount, cellYCount);

    auto window = sf::RenderWindow(
            sf::VideoMode(800, 800, 32),
            "CMake SFML Project",
            sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(144);

    sf::Color borderColor(170, 108, 57);
    sf::Color openCellColor(34, 102, 102);
    sf::Color closedCellColor(170, 57, 57);

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear(sf::Color::White);

        float cellXSize = window.getSize().x / cellXCount;
        float cellYSize = window.getSize().y / cellYCount;
        sf::Vector2f cellSize(cellXSize, cellYSize);

        for (int ix = 0; ix < cellXCount; ix++)
        {
            for (int jx = 0; jx < cellXCount; jx++)
            {
                sf::RectangleShape outerCell(cellSize);
                outerCell.setPosition(sf::Vector2f(cellXSize * ix, cellYSize * jx));

                if (game.getCellStatus(ix, jx))
                {
                    outerCell.setFillColor(openCellColor);
                }
                else
                {
                    outerCell.setFillColor(closedCellColor);
                }

                outerCell.setOutlineColor(borderColor);
                outerCell.setOutlineThickness(2.f);

                window.draw(outerCell);
            }
        }

        window.display();

        game.update();
    }
}