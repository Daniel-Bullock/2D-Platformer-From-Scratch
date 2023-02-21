#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream> 
#include "Player.hpp"
#include "TileMap.hpp"
#include <math.h>


Player::Player()
{
    // load the player texture
    if (!playerTexture.loadFromFile("Textures/Player/player16bit.png"))
    {
        return;
    }

    if (!orbTexture.loadFromFile("Textures/Orb/orbtest1.png"))
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
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        jumpReleased = true;
    }
    //prevents holding jump and instantly rejumping
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isJumping && !isFalling && jumpReleased) 
    {
        isJumping = true;
        velocityY = initialVelocityY;  //initial velocity
        jumpReleased = false;
    }
    if (isJumping)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && jumpTimer <= jumpDuration)
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

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isOrbShooting)
    { 
        shootOrb(window, tiles);
    }

    if (isOrbShooting){
        sf::FloatRect orbBoundsX = orbSprite.getGlobalBounds();
        sf::FloatRect orbBoundsY = orbSprite.getGlobalBounds();
        orbBoundsX.left += orbVx;
        orbBoundsY.top += orbVy;
        
        sf::Vector2<bool> orbCollider = tiles.collisions(orbBoundsX, orbBoundsY);
        if(orbCollider.x == true || orbCollider.y == true){
            isOrbShooting = false;
        }
        orbSprite.move(orbVx, orbVy);
    }    
}

void Player::shootOrb(sf::RenderWindow& window, TileMap& tiles)
{
    if (!allowShooting && shotClock.getElapsedTime().asSeconds() > timeBetweenShooting) {
        allowShooting = true;
    }
    if(allowShooting){
        orbSprite.setTexture(orbTexture);
        orbSprite.setPosition(playerSprite.getPosition().x + playerSprite.getGlobalBounds().height/2, playerSprite.getPosition().y + playerSprite.getGlobalBounds().width/2);

        // calculate the direction from the player's position to the mouse position
        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        float dx = mousePosition.x - (playerSprite.getPosition().x + playerSprite.getGlobalBounds().width/2);
        float dy = mousePosition.y - (playerSprite.getPosition().y + playerSprite.getGlobalBounds().height/2);
        float angle = atan2(dy,dx);
        //std::cout<<angle<<'\n';

        // set the initial velocity of the orb in the calculated direction
        
        orbVx = cos(angle) * orbSpeed;
        orbVy = sin(angle) * orbSpeed;

        isOrbShooting = true;
        allowShooting = false;

        shotClock.restart();
    }
}

void Player:: drawOrb(sf::RenderWindow &window)
{
    window.draw(orbSprite);
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







