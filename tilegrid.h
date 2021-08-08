#pragma once 
#include <array>
#include <random>
#include <iostream>
#include <iomanip>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

struct Tile {
    sf::Sprite sprite;
    static constexpr int size = 40;
    bool flaged = false;
    bool isMine = false;
    bool revealed = false;
    int minesAround = 0;
};

class TileGrid : public sf::Drawable {
    public:
        TileGrid(sf::Window& win):
            win(win),
            tiles()
        {
            grid.setSize(sf::Vector2f(400, 400));
            grid.setPosition(sf::Vector2f(0, 0));

            // Load game tile set (20x20)
            tileset.loadFromFile("./minesweeper.png");

            regenerateMines();
        }

        void regenerateMines(){
            // Reset
            // Set properties
            for(std::size_t i = 0; i < tiles.size(); i++) {
                for(std::size_t j = 0; j < tiles[0].size(); j++) {
                    // position - x40
                    // texture position - x20
                    Tile& tile = tiles[i][j];
                    tile.sprite.setTexture(tileset);
                    tile.sprite.setTextureRect(sf::IntRect(0, 0, 20, 20));
                    tile.sprite.setScale(sf::Vector2f(2.f, 2.f));
                    tile.sprite.setPosition(sf::Vector2f(j * 40.f, i * 40.f));
                    // Reset tile properties
                    tile.isMine = false;
                    tile.revealed = false;
                    tile.minesAround = 0;
                    tile.flaged = false;
                }
            }
            prevTile = nullptr;

            // Generate 10% mines
            std::random_device rd;
            std::uniform_int_distribution<int> dist{0, 9};
            for(auto& row : tiles){
                for(auto& tile : row){
                    tile.minesAround = 0;
                    // Probability: {0, 1, 2, 3} == 0 // 25%
                    if(dist(rd) == 0){ 
                        tile.isMine = true;
                    }
                }
            }

            // Count mines around each tile
            for(std::size_t i = 0; i < tiles.size(); i++){
                for(std::size_t j = 0; j < tiles[0].size(); j++){
                    if(!tiles[i][j].isMine){
                        /*  1  3  2
                         *  +-----+
                         * 7|  9  |8
                         *  |     | 
                         *  +-----+
                         *  4  6  5
                         */
                        if(i == 0){
                            if(j == 0){
                                // 1
                                tiles[i][j].minesAround += tiles[i][j + 1].isMine ? 1 : 0;
                                tiles[i][j].minesAround += tiles[i + 1][j + 1].isMine ? 1 : 0;
                                tiles[i][j].minesAround += tiles[i + 1][j].isMine ? 1 : 0;
                                continue;
                            } else if(j == tiles[0].size() - 1){
                                // 2
                                tiles[i][j].minesAround += tiles[i][j - 1].isMine ? 1 : 0;
                                tiles[i][j].minesAround += tiles[i + 1][j - 1].isMine ? 1 : 0;
                                tiles[i][j].minesAround += tiles[i + 1][j].isMine ? 1 : 0;
                                continue;
                            }
                            // 3
                            tiles[i][j].minesAround += tiles[i][j - 1].isMine ? 1 : 0;
                            tiles[i][j].minesAround += tiles[i + 1][j - 1].isMine ? 1 : 0;
                            tiles[i][j].minesAround += tiles[i + 1][j].isMine ? 1 : 0;
                            tiles[i][j].minesAround += tiles[i + 1][j + 1].isMine ? 1 : 0;
                            tiles[i][j].minesAround += tiles[i][j + 1].isMine ? 1 : 0;
                            continue;
                        } else if(i == tiles.size() - 1){
                            if(j == 0){
                                // 4
                                tiles[i][j].minesAround += tiles[i][j + 1].isMine ? 1 : 0;
                                tiles[i][j].minesAround += tiles[i - 1][j + 1].isMine ? 1 : 0;
                                tiles[i][j].minesAround += tiles[i - 1][j].isMine ? 1 : 0;
                                continue;
                            } else if(j == tiles[0].size() - 1){
                                // 5
                                tiles[i][j].minesAround += tiles[i][j - 1].isMine ? 1 : 0;
                                tiles[i][j].minesAround += tiles[i - 1][j - 1].isMine ? 1 : 0;
                                tiles[i][j].minesAround += tiles[i - 1][j].isMine ? 1 : 0;
                                continue;
                            }
                            // 6
                            tiles[i][j].minesAround += tiles[i][j - 1].isMine ? 1 : 0;
                            tiles[i][j].minesAround += tiles[i - 1][j - 1].isMine ? 1 : 0;
                            tiles[i][j].minesAround += tiles[i - 1][j].isMine ? 1 : 0;
                            tiles[i][j].minesAround += tiles[i - 1][j + 1].isMine ? 1 : 0;
                            tiles[i][j].minesAround += tiles[i][j + 1].isMine ? 1 : 0;
                            continue;
                        }

                        if(j == 0){
                            // 7
                            tiles[i][j].minesAround += tiles[i - 1][j].isMine ? 1 : 0;
                            tiles[i][j].minesAround += tiles[i - 1][j + 1].isMine ? 1 : 0;
                            tiles[i][j].minesAround += tiles[i][j + 1].isMine ? 1 : 0;
                            tiles[i][j].minesAround += tiles[i + 1][j + 1].isMine ? 1 : 0;
                            tiles[i][j].minesAround += tiles[i + 1][j].isMine ? 1 : 0;
                            continue;
                        } else if(j == tiles[0].size() - 1){
                            // 8
                            tiles[i][j].minesAround += tiles[i - 1][j].isMine ? 1 : 0;
                            tiles[i][j].minesAround += tiles[i - 1][j - 1].isMine ? 1 : 0;
                            tiles[i][j].minesAround += tiles[i][j - 1].isMine ? 1 : 0;
                            tiles[i][j].minesAround += tiles[i + 1][j - 1].isMine ? 1 : 0;
                            tiles[i][j].minesAround += tiles[i + 1][j].isMine ? 1 : 0;
                            continue;
                        }
                        
                        // 9
                        tiles[i][j].minesAround += tiles[i][j - 1].isMine ? 1 : 0;
                        tiles[i][j].minesAround += tiles[i - 1][j - 1].isMine ? 1 : 0;
                        tiles[i][j].minesAround += tiles[i - 1][j].isMine ? 1 : 0;
                        tiles[i][j].minesAround += tiles[i - 1][j + 1].isMine ? 1 : 0;
                        tiles[i][j].minesAround += tiles[i][j + 1].isMine ? 1 : 0;
                        tiles[i][j].minesAround += tiles[i + 1][j + 1].isMine ? 1 : 0;
                        tiles[i][j].minesAround += tiles[i + 1][j].isMine ? 1 : 0;
                        tiles[i][j].minesAround += tiles[i + 1][j - 1].isMine ? 1 : 0;
                    }
                }
            }
            
            logMinesCount();
        }

        void logMinesCount(){
            for(auto& row : tiles){
                for(auto& tile : row){
                    std::clog << std::setw(2) << tile.minesAround;
                }
                std::clog.put('\n');
            }
        }

        void onEvent(sf::Event& e) {
            if (!gameplay) {
                return;
            }
            // Dispatch event
            switch(e.type){
                case sf::Event::MouseButtonPressed:
                    // Click on tile
                    if(grid.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)){
                        int x = e.mouseButton.x / Tile::size;
                        int y = e.mouseButton.y / Tile::size;
                        onClick(x, y);
                    }
                    break;

                case sf::Event::MouseMoved:
                    if (grid.getGlobalBounds().contains(e.mouseMove.x, e.mouseMove.y)) {
                        int x = e.mouseMove.x / Tile::size;
                        int y = e.mouseMove.y / Tile::size;
                        onMove(x, y);
                    }
                    break;
            }
        }

        void onClick(int x, int y) {
            if (flagging) {
                tiles[y][x].flaged = true;
                return;
            }

            // Game over
            if (tiles[y][x].isMine) {
                gameOver();
            }
            tiles[y][x].revealed = true;

            updateTile(tiles[y][x]);

            // Reveal empty tiles near
            /*
            if(tiles[y][x].minesAround == 0){
                // Check for empty tiles near
                clickTileIfEmpty(x - 1, y);
                clickTileIfEmpty(x - 1, y - 1);
                clickTileIfEmpty(x, y - 1);
                clickTileIfEmpty(x + 1, y - 1);
                clickTileIfEmpty(x + 1, y);
                clickTileIfEmpty(x + 1, y + 1);
                clickTileIfEmpty(x, y + 1);
                clickTileIfEmpty(x - 1, y + 1);
            }
            */
        }

        /*
        void clickTileIfEmpty(int x, int y){
            if(y >= 0 && y < tiles.size()){
                if(x >= 0 && x < tiles[0].size()){
                    if(tiles[y][x].minesAround == 0 ? true : false){
                        onClick(x, y);
                    }
                }
            }
        }
        */

        void onMove(int x, int y) {
            // Whether moved to the other tile
            if (prevTile != &tiles[y][x]) {
                // tile.selected == true
                tiles[y][x].sprite.setTextureRect(sf::IntRect(0, 20, 20, 20));

                // Change previous tile
                if (prevTile != nullptr) {
                    updateTile(*prevTile);
                }
            }

            // Tile is now previous
            prevTile = &tiles[y][x];
        }

        void gameOver() {
            /* win.close(); */
            gameplay = false;
            firstLife = false;
        }

        inline void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            // Apply render states
            //states.transform *= getTransform();
            //states.texture = &m_texture;

            for (auto& row : tiles) {
                for (auto& tile : row) {
                    target.draw(tile.sprite);
                }
            }
        }

        void resetTile(Tile& tile);

        void updateTile(Tile& tile) {
            if (tile.flaged) {
                // Set flaged style
                tile.sprite.setTextureRect(sf::IntRect(0, 40, 20, 20));
                
            } else if (tile.revealed) {
                if (tile.isMine) {
                    // You will be already dead
                } else {
                    // Here with the number displayed
                    tile.sprite.setTextureRect(sf::IntRect(tile.minesAround * 20 + 20, 0, 20, 20));
                }
            } else {
                if (tile.isMine) {
                } else {
                }
                // Just here
                tile.sprite.setTextureRect(sf::IntRect(0, 0, 20, 20));
            }
        }

        void playClicked() {
            if (firstLife) {
                gameplay = true;
            }
            else {
                gameplay = true;
                regenerateMines();
            }
        }

        void resetClicked() {
            regenerateMines();
        }

        void flagClicked(bool toFlag) {
            flagging = toFlag;
        }

        void exitClicked() {
            gameOver();
            win.close();
        }

    private:
        sf::Window& win;
        sf::RectangleShape grid;
        // Tile info
        std::array<std::array<Tile, 10>, 10> tiles;
        sf::Texture tileset;
        // Previous selected tile
        Tile* prevTile = nullptr;
        bool gameplay = false;
        bool flagging = false;
        bool firstLife = true;
};
