#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream> 
#include <math.h>

#include "TileMap.hpp"
#include <Global.hpp>
#include <Enemy.hpp>

TileMap::TileMap(int MAP_WIDTH, int MAP_HEIGHT, int TILE_SIZE, std::vector<Enemy> &enemies){
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
    std::vector<std::vector<sf::FloatRect> > tileBoundstemp(MAP_HEIGHT); 

    //std::vector<Enemy> enemiesTemp;     


    for (int i = 0; i < mapHeight; i++)
    {
        tileBoundstemp[i].resize(MAP_WIDTH);
        for (int j = 0; j < mapWidth; j++)
        {
            if(tilemap[i][j] == 1) 
            {
                tileBoundstemp[i][j] = sf::FloatRect(j * tileSize, i * tileSize, tileSize, tileSize);
            }
            else if(tilemap[i][j] == 9)
            {
                //make new enemy at this location and add to list of enemies
                //enemies.push_back(Enemy(j * tileSize,i * tileSize, 0.016, enemyTexture, tileSize));
                //enemiesTemp.push_back(Enemy(j * tileSize,i * tileSize, 0.016, enemyTexture, tileSize));
                enemies.push_back(Enemy(j * tileSize,i * tileSize, 0.016, tileSize));
            }
        }
    }
    tileBounds = tileBoundstemp;
    //enemies = enemiesTemp;
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

void TileMap::draw(sf::RenderWindow &window, sf::View &view)
{   
    int topIdx = std::max(int((view.getCenter().y - 180)/16), 0);
    int bottomIdx = std::min(int((view.getCenter().y + 180)/16) + 1, MAP_HEIGHT);
    for (int x = topIdx; x < bottomIdx; x++)
    {
        for (int y = 0; y < mapWidth; y++)
        {
            int tileNumber = tilemap[x][y];
            if(tileNumber == 1){
                window.draw(tiles[tileNumber]);
                window.draw(tiles[tileNumber]);
                tiles[tileNumber].setPosition(y * tileSize, x * tileSize);
            }
            
        }
    }
}

/*
void TileMap::updateEnemies(sf::View &view, std::vector<std::vector<Enemy>> &enemies){
    int topIdx = std::max(int((view.getCenter().y - 180)/16), 0);
    int bottomIdx = std::min(int((view.getCenter().y + 180)/16) + 1, MAP_HEIGHT);

    for (int x = topIdx; x < bottomIdx; x++)
    {
        for (int y = 0; y < mapWidth; y++)
        {
            if (tilemap[x][y] ==  9)
            {
                
                Enemy currEnemy = enemies[x][y];
                //std::cout<<currEnemy.getPos().x<<"\n";
                //std::cout<<currEnemy.position<<"\n";
                currEnemy.update(tilemap, TILE_SIZE);
            }
        }
    }
}*/


