#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream> 
#include "Player.hpp"
#include "TileMap.hpp"


Player::Player()
{
    // load the player texture
    if (!playerTexture.loadFromFile("Textures/Player/player16bit.png"))
    {
        return;
    }
    // set up the player sprite
    playerSprite.setTexture(playerTexture);

    // set the starting position of the player
    playerSprite.setPosition(50, 64);
}

void Player::update(sf::RenderWindow &window, TileMap& tiles)
{
    //Jumping
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        jumpReleased = true;
    }
    //prevents holding jump and instantly rejumping
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !isJumping && !isFalling && jumpReleased) 
    {
        isJumping = true;
        velocityY = initialVelocityY;  //initial velocity
        jumpReleased = false;
    }
    if (isJumping)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && jumpTimer <= jumpDuration)
        {
            velocityY = initialVelocityY;
            jumpTimer +=1;
        }else
        {
            isJumping = false;
            isFalling = true;
            jumpTimer = 0;
        }
    }else //gravity
    {   
        if(isFalling){
            velocityY += gravity;
            velocityY = std::min(velocityY, maxVelocityY);
        }
    }

    //Left and right movement and friction
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {   
        velocityX = std::max(velocityX - accelerationX, -maxVelocityX);
    }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        velocityX = std::min(velocityX + accelerationX, maxVelocityX);
    }else if(velocityX>0)
    {
        velocityX = std::max(0.0f,velocityX - accelerationX);
    }else if(velocityX<0)
    {
        velocityX = std::min(0.0f,velocityX + accelerationX);
    }

    //collision check - separate for x and y plane
    sf::FloatRect playerBoundsX = playerSprite.getGlobalBounds();
    sf::FloatRect playerBoundsY = playerSprite.getGlobalBounds();
    playerBoundsX.left += velocityX;
    playerBoundsY.top += velocityY;
    
    sf::Vector2<bool> collider = tiles.collisions(playerBoundsX,playerBoundsY);

    isFalling = true; //assume player is falling, if it's on ground, then set it to not falling
    doBounce = false;  //bouncing off walls

    if (collider.x == true){
        doBounce = true;
    }
    if (collider.y == true){
        if(velocityY>=0){
            isFalling = false;
            velocityY = 0.0;
        }else{
            isFalling = true;
            isJumping = false;
            velocityY = velocityY/2;   //hitting ceiling lowers velocity
            jumpTimer = 0;
        }
        velocityY = 0.0;
    } 

    if(doBounce){
        velocityX = -velocityX/1.1;
    }
}

void Player::draw(sf::RenderWindow &window)
{
    window.draw(playerSprite);
}

sf::Vector2f Player::getPosition()
{
    return playerSprite.getPosition();
}

void Player::setPosition(sf::Vector2f pos)
{
    playerSprite.setPosition(pos);
}

void Player::move()
{
    playerSprite.move(velocityX,velocityY);
}

sf::FloatRect Player::getGlobalBounds()
{
    return playerSprite.getGlobalBounds();
}







