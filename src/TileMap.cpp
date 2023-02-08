#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream> 
#include "TileMap.hpp"


TileMap::TileMap(int MAP_WIDTH, int MAP_HEIGHT, int TILE_SIZE){
    tileSize = TILE_SIZE;
    mapHeight = MAP_HEIGHT;
    mapWidth = MAP_WIDTH;
    if (!tileset.loadFromFile("Textures/Tiles/tileset.png")){
        std::cout<<"Couldn't load tileset";
    }
    
    for (int i = 0; i < tileset.getSize().x / tileSize; i++)
    {
        sf::Sprite sprite;
        sprite.setTexture(tileset);
        sprite.setTextureRect(sf::IntRect(i * tileSize, 0, tileSize, tileSize));
        tiles.push_back(sprite);
    }    
    
    //create collision boxes for tiles
    
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            if(tilemap[i][j] == 1)
            {
    
                tileBounds.push_back(sf::FloatRect(j * tileSize, i * tileSize, tileSize, tileSize));
            }
        }
    }

}

void TileMap::draw(sf::RenderWindow &window)
{
    for (int x = 0; x < mapHeight; x++)
        {
            for (int y = 0; y < mapWidth; y++)
            {
                int tileNumber = tilemap[x][y];
                window.draw(tiles[tileNumber]);
                tiles[tileNumber].setPosition(y * tileSize, x * tileSize);
            }
        }
}



