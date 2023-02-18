#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream> 
#include "TileMap.hpp"
#include <math.h>


TileMap::TileMap(int MAP_WIDTH, int MAP_HEIGHT, int TILE_SIZE){
    tileSize = TILE_SIZE;
    mapHeight = MAP_HEIGHT;
    mapWidth = MAP_WIDTH;
    
    if (!tileset.loadFromFile("Textures/Tiles/tileset16.png")){
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
    //std::vector<std::vector<sf::FloatRect> > tileBoundstemp(23);  
    std::vector<std::vector<sf::FloatRect> > tileBoundstemp(28);      

    for (int i = 0; i < mapHeight; i++)
    {
        tileBoundstemp[i].resize(40);
        for (int j = 0; j < mapWidth; j++)
        {
            if(tilemap[i][j] == 1)
            {
                tileBoundstemp[i][j] = sf::FloatRect(j * tileSize, i * tileSize, tileSize, tileSize);
            }
        }
    }

    tileBounds = tileBoundstemp;
}

sf::Vector2<bool> TileMap::collisions(sf::FloatRect boundsX, sf::FloatRect boundsY){
    sf::Vector2<bool> collider;
    for(unsigned short a = std::max<short>(0,floor(boundsX.left/tileSize)); a <= std::min<short>(floor((ceil(boundsX.left + boundsX.width)-1)/tileSize),tileSize*mapWidth); a++){
        for(unsigned short b = std::max<short>(0,floor(boundsY.top/tileSize)); b <= std::min<short>(floor((ceil(boundsY.top + boundsY.height)-1)/tileSize),tileSize*mapHeight); b++){
            sf::FloatRect tilebound = tileBounds[b][a];
            if (boundsX.intersects(tilebound)){
                collider.x = true;
            }
            if (boundsY.intersects(tilebound)){
                collider.y = true;
            } 
        }
    }
    return collider;
}

void TileMap::draw(sf::RenderWindow &window)
{

    // instead of 0 and mapheight have input that changes it

    

    //std::cout<<yDiff <<'\n';
    
    //for (int x = 0; x < mapHeight; x++)
    for (int x = 0; x < mapHeight; x++)
        {
            //for (int y = 0; y < mapWidth; y++)
            for (int y = 0; y < mapWidth; y++)
            {
                int tileNumber = tilemap[x][y];
                window.draw(tiles[tileNumber]);
                tiles[tileNumber].setPosition(y * tileSize, x * tileSize);
            }
        }
}



