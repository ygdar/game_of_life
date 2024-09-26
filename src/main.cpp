#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <string>

#include <SFML/Graphics.hpp>

#include "game_of_life.h"

std::chrono::duration<double, std::milli> ms_double;
std::condition_variable frame_update_completed;
std::mutex frame_lock;

int main()
{
    int cellXCount = 160;
    int cellYCount = 160;

    auto game = gol::GameOfLife(cellXCount, cellYCount);
    game.setRandomSeed();

    auto game_loop = std::thread([&game]() {
        for(;;)
        {
            auto start = std::chrono::high_resolution_clock::now();
            game.prepare();
            auto end = std::chrono::high_resolution_clock::now();

            std::unique_lock lock(frame_lock);
            frame_update_completed.wait(lock);

            ms_double = end - start;
            game.update();

            lock.unlock();
        }
    });

    auto window = sf::RenderWindow(
            sf::VideoMode(cellXCount * 4, cellYCount * 4, 32),
            "CMake SFML Project",
            sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(100);

    sf::Color borderColor(112, 104, 104);
    sf::Color openCellColor(164, 151, 151);
    sf::Color closedCellColor(27, 14, 14);

    sf::Font font;
    auto font_loaded = font.loadFromFile("euler.otf");

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        float cellXSize = window.getSize().x / cellXCount;
        float cellYSize = window.getSize().y / cellYCount;
        sf::Vector2f cellSize(cellXSize, cellYSize);

        sf::VertexArray va(sf::PrimitiveType::Triangles);

        for (int ix = 0; ix < cellXCount; ix++)
        {
            for (int jx = 0; jx < cellYCount; jx++)
            {
                sf::RectangleShape outerCell(cellSize);
                outerCell.setPosition(sf::Vector2f(cellXSize * ix, cellYSize * jx));

                if (game.getCellStatus(ix, jx))
                {
                    size_t base_index = va.getVertexCount() - 1;
                    va.resize(va.getVertexCount() + 6);

                    va[base_index + 1].position = sf::Vector2f(cellXSize * ix, cellYSize * jx);
                    va[base_index + 2].position = sf::Vector2f(cellXSize * (ix + 1), cellYSize * jx);
                    va[base_index + 3].position = sf::Vector2f(cellXSize * ix, cellYSize * (jx + 1));

                    va[base_index + 4].position = sf::Vector2f(cellXSize * (ix + 1), cellYSize * (jx + 1));
                    va[base_index + 5].position = sf::Vector2f(cellXSize * (ix + 1), cellYSize * jx);
                    va[base_index + 6].position = sf::Vector2f(cellXSize * ix, cellYSize * (jx + 1));
                }
            }
        }

        for (int ix = 0; ix < va.getVertexCount(); ix++)
        {
            va[ix].color = openCellColor;
        }

        {
            std::lock_guard lock(frame_lock);

            window.clear(closedCellColor);
            window.draw(va);

            sf::Text measurement;
            measurement.setFont(font);
            measurement.setString(std::to_string(ms_double.count()) + "ms");
            measurement.setPosition(20, 20);
            measurement.setCharacterSize(16);
            measurement.setOutlineColor(sf::Color::Yellow);
            measurement.setOutlineThickness(1);
            window.draw(measurement);
        }

        frame_update_completed.notify_one();
        window.display();
    }
}