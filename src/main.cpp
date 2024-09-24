#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <Eigen/Dense>

#include "game_of_life.h"

int main()
{
    int cellXCount = 320;
    int cellYCount = 160;

    auto game = gol::GameOfLife(cellXCount, cellYCount);
    game.setRandomSeed();

    auto window = sf::RenderWindow(
            sf::VideoMode(1600, 800, 32),
            "CMake SFML Project",
            sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(100);

    sf::Color borderColor(112, 104, 104);
    sf::Color openCellColor(164, 151, 151);
    sf::Color closedCellColor(27, 14, 14);

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
            for (int jx = 0; jx < cellYCount; jx++)
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
                outerCell.setOutlineThickness(0.5f);

                window.draw(outerCell);
            }
        }

        window.display();

        game.update();
    }
}