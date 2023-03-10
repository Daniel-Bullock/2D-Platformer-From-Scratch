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

void Player::update(sf::RenderWindow &window, TileMap& tiles, std::vector<Enemy>& enemies)
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
    
    sf::Vector2<bool> collider = tiles.collisionsXY(playerBoundsX,playerBoundsY);

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


    //Handle orb shooting
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isOrbShooting && !isMouseClicked)
    { 
        isMouseClicked = true;
        shootingTimer.restart();
    }

    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && isMouseClicked) {
        float duration = std::min(shootingTimer.getElapsedTime().asSeconds(), maxHoldTime);
        isMouseClicked = false;
        shootOrb(window, tiles, duration);
    }

    if (isOrbShooting){
        orbVx = orbVx * airResistance;
        orbVy = orbVy + gravityOrb;
        
        sf::FloatRect orbBounds = orbSprite.getGlobalBounds();
        orbBounds.left += orbVx;
        orbBounds.top += orbVy;

        //colliding with tiles
        bool orbCollider = tiles.collisions(orbBounds);
        if(orbCollider){
            isOrbShooting = false;
        }
        
        //colliding with enemies
        for(int i = 0; i < enemies.size(); i++){
            if(orbBounds.intersects(enemies[i].sprite.getGlobalBounds())){
                //How to delete???? idk how to do safely without memory leaks :'(
                    //vector of smart pointers to enemies??????????????????????????????????????s


                    
                auto iterator = enemies.begin()+i;
                enemies.erase(iterator);
            }
        }
        orbSprite.move(orbVx, orbVy);
    }    
}

void Player::shootOrb(sf::RenderWindow& window, TileMap& tiles, float duration)
{
    if (!allowShooting && shotClock.getElapsedTime().asSeconds() > timeBetweenShooting) {
        allowShooting = true;
    }
    if(allowShooting){
        orbSprite.setTexture(orbTexture);
        orbSprite.setPosition(playerSprite.getPosition().x + playerSprite.getGlobalBounds().height/2, playerSprite.getPosition().y + playerSprite.getGlobalBounds().width/2);

        // calculate the direction from the player's position to the mouse position
        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        float initialSpeed = minOrbSpeed + (maxOrbSpeed - minOrbSpeed) * (duration / maxHoldTime);

        float dx = mousePosition.x - (playerSprite.getPosition().x + playerSprite.getGlobalBounds().width/2);
        float dy = mousePosition.y - (playerSprite.getPosition().y + playerSprite.getGlobalBounds().height/2);
        float angle = atan2(dy,dx);
        
        orbVx = cos(angle) * initialSpeed;
        orbVy = sin(angle) * initialSpeed;

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







