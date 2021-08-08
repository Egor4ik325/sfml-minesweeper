#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include "tilegrid.h"

#include <iostream>

int main() {
    constexpr int width = 400;
    constexpr int height = 440;
    sf::RenderWindow window(sf::VideoMode(width, height, 32), "Minesweeper", sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // Tile map
    TileGrid grid(window);

    // Buttons
    /* sf::RectangleShape resetButton; */
    /* resetButton.setSize(sf::Vector2f(80, 40)); */
    /* resetButton.setPosition(sf::Vector2f(0, 400)); */
    /* resetButton.setFillColor(sf::Color::Magenta); */

    sf::Texture tileset;
    tileset.loadFromFile("minesweeper.png");

    sf::Sprite playButton(tileset, sf::IntRect(20, 20, 40, 20));
    /* playButton.setSize(sf::Vector2f(80, 40)); */
    playButton.setScale(sf::Vector2f(2, 2));
    playButton.setPosition(sf::Vector2f(0, 400));

    sf::Sprite resetButton(tileset, sf::IntRect(60, 20, 40, 20));
    resetButton.setScale(sf::Vector2f(2, 2));
    resetButton.setPosition(sf::Vector2f(80, 400));

    bool flagging = false;
    sf::Sprite flagButton(tileset, sf::IntRect(100, 20, 40, 20));
    flagButton.setScale(sf::Vector2f(2, 2));
    flagButton.setPosition(sf::Vector2f(160, 400));

    sf::Sprite exitButton(tileset, sf::IntRect(140, 20, 40, 20));
    exitButton.setScale(sf::Vector2f(2, 2));
    exitButton.setPosition(sf::Vector2f(240, 400));

    // Game loop
    while(window.isOpen()){
        // Handle events
        sf::Event event{};
        // Poll all available events
        while(window.pollEvent(event)){
            // Window event
            if(event.type == sf::Event::Closed){
                window.close();
            }
            // Button event (UI)
            if(event.type == sf::Event::MouseButtonPressed){
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                if (playButton.getGlobalBounds().contains(mousePos)) {
                    grid.playClicked();
                } else if (resetButton.getGlobalBounds().contains(mousePos)) {
                    grid.resetClicked();
                } else if (flagButton.getGlobalBounds().contains(mousePos)) {
                    // Change button style
                    if (!flagging) {
                        flagButton.setTextureRect(sf::IntRect(100, 40, 40, 20));
                        // Send event
                        grid.flagClicked(!flagging);
                        flagging = true;
                    } else {
                        flagButton.setTextureRect(sf::IntRect(100, 20, 40, 20));
                        grid.flagClicked(!flagging);
                        flagging = false;
                    }

                } else if (exitButton.getGlobalBounds().contains(mousePos)) {
                    grid.exitClicked();
                }
            }
            // Tilegrid event
            grid.onEvent(event);
        }

        // Update state:
        // no loop updates, the game is fully event based, nothing happens
        // until user generate event, except for drawing which is happen every 
        // frame for some reason (probably it shouldn't)

        // Render graphics
        window.clear();
        // Constant draw methods
        window.draw(grid);
        window.draw(playButton);
        window.draw(resetButton);
        window.draw(flagButton);
        window.draw(exitButton);

        window.display();
    }

    return 0;
}
