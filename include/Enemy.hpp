#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <SFML/Graphics.hpp>

class Enemy {
    public:
        //Enemy();
        //Enemy(float x_, float y_, float speed, sf::Texture& texture_, int tileSize);
        Enemy(float x_, float y_, float speed, int tileSize);

        sf::Vector2f getPos();
        
        void update(std::vector<std::vector<int>>& tileMap, int tileSize);

        void draw(sf::RenderWindow& window);

        sf::Sprite sprite;
        

        float x;
        float y;

    private:
        float movementSpeed;
        int direction;

        sf::Texture texture;
   
};




#endif