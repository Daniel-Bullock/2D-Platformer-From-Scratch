#ifndef _Player_H_
#define _Player_H_

#include "TileMap.hpp"


class Player
{
public:
    Player();

    void update(sf::RenderWindow &window, TileMap& tiles);
    void draw(sf::RenderWindow &window);
    void move();

    void shootOrb(sf::RenderWindow& window, TileMap& tiles);
    void drawOrb(sf::RenderWindow &window);

    sf::Vector2f getPosition();
    sf::FloatRect getGlobalBounds();

    void setPosition(sf::Vector2f pos);

    const float gravity = 0.0003f/2;
    const float maxVelocityY = 0.35;
    const float initialVelocityY = -0.2/2;

    const float accelerationX = 0.0001f/2;
    const float maxVelocityX = 0.2f/2;

    float velocityY = 0.0f; 
    float velocityX = 0.0f;

    bool isJumping = false;
    bool isFalling = false;
    bool jumpReleased = true; //check if W has been released to prevent holding W and instant jumping

    int jumpTimer = 0;
    int jumpDuration = 800;

    bool doBounce = false;  //bouncing off walls

    float orbSpeed = 0.11f;
    float timeBetweenShooting = 0.8;
    bool isOrbShooting = false;
    bool allowShooting = true;

private:
    sf::Texture playerTexture;
    sf::Sprite playerSprite;
    
    sf::Sprite orbSprite;
    sf::Texture orbTexture;
    float orbVx;
    float orbVy;

    sf::Clock shotClock;
};



#endif