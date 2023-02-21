#include <SFML/Graphics.hpp>
#include "Enemy.hpp"
#include <iostream> 
#include <Global.hpp>

//Enemy::Enemy(float x_, float y_, float speed, sf::Texture& texture_, int tileSize) {
Enemy::Enemy(float x_, float y_, float speed, int tileSize) {
    // Initialize enemy properties
    sprite.setPosition(x_, y_);
    movementSpeed = speed;
    direction = 1; // 1 for right, -1 for left
    x = x_;
    y = y_;
}

sf::Vector2f Enemy::getPos(){
    return sprite.getPosition();
}

void Enemy::update(std::vector<std::vector<int>>& tileMap, int tileSize) {
    // Check for collisions with walls and edges respectively also making sure check is within map (1 = right)
    if ((direction == 1 && (((int)(y / tileSize) < MAP_HEIGHT &&  (int)((x + sprite.getGlobalBounds().width) / tileSize) <MAP_WIDTH && tileMap[(int)(y / tileSize)][(int)((x + sprite.getGlobalBounds().width) / tileSize)] == 1) || 
                            ( (int)((y + sprite.getGlobalBounds().height) / tileSize) < MAP_HEIGHT && tileMap[(int)((y + sprite.getGlobalBounds().height) / tileSize)][(int)((x + sprite.getGlobalBounds().width) / tileSize)] == 0))) ||
        (direction == -1 && (tileMap[(int)(y / tileSize)][(int)((x) / tileSize)] == 1 ||
                              ((int)((y + sprite.getGlobalBounds().height) / tileSize) < MAP_HEIGHT  &&  tileMap[(int)((y + sprite.getGlobalBounds().height) / tileSize)][(int)((x) / tileSize)] == 0)))) {
        // Change direction if the enemy hits a wall or edge
        direction *= -1;
    }

    sprite.move(movementSpeed * direction, 0.f);

    x += movementSpeed * direction; //update position
}

void Enemy::draw(sf::RenderWindow& window) {
    // Draw the enemy sprite to the window
    texture.loadFromFile("Textures/Player/slimetest1.png");
    sprite.setTexture(texture);
    window.draw(sprite);
}





