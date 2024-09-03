#include <random>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

int main()
{
    auto window = sf::RenderWindow(
            sf::VideoMode(800, 800, 32),
            "CMake SFML Project",
            sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(144);

    sf::Color borderColor(170, 108, 57);

    std::vector<sf::Color> openCellColors = {
            sf::Color(102, 153, 153),
            sf::Color(64, 127, 127),
            sf::Color(34, 102, 102),
            sf::Color(13, 77, 77),
            sf::Color(0, 51, 51),
    };

    std::vector<sf::Color> closedCellColors = {
            sf::Color(255, 170, 170),
            sf::Color(212, 106, 106),
            sf::Color(170, 57, 57),
            sf::Color(128, 21, 21),
            sf::Color(85, 0, 0),
    };

    sf::Color openCellColor(34, 102, 102);
    sf::Color closedCellColor(170, 57, 57);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution distr(0, 1);

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

        int cellXCount = 80;
        int cellYCount = 80;

        float cellXSize = window.getSize().x / cellXCount;
        float cellYSize = window.getSize().y / cellYCount;
        sf::Vector2f cellSize(cellXSize, cellYSize);

        for (int ix = 0; ix < cellXCount; ix++)
        {
            for (int jx = 0; jx < cellXCount; jx++)
            {
                sf::RectangleShape outerCell(cellSize);
                outerCell.setPosition(sf::Vector2f(cellXSize * ix, cellYSize * jx));

                int c = abs((int)((sin(ix) + sin(jx)) * 256));

                outerCell.setFillColor(sf::Color(c, c, c));
                outerCell.setOutlineColor(borderColor);
                outerCell.setOutlineThickness(1.f);

                window.draw(outerCell);
            }
        }

        window.display();
    }
}