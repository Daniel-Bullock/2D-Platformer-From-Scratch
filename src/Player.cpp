#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream> 
#include "Player.hpp"


Player::Player()
{
    // load the player texture
    if (!playerTexture.loadFromFile("Textures/Player/playertest3.png"))
    {
        return;
    }
    // set up the player sprite
    playerSprite.setTexture(playerTexture);
    //playerSprite.setOrigin(playerTexture.getSize().x / 2, playerTexture.getSize().y / 2);

    // set the starting position of the player
    playerSprite.setPosition(50, 64);

    // set the player's velocity
    velocity = sf::Vector2f(0, 0);
}

/*void Player::update(float dx,float dy)
{
    //nothing for now


}*/

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

void Player::move(float x, float y)
{
    playerSprite.move(x,y);
}

sf::FloatRect Player::getGlobalBounds()
{
    return playerSprite.getGlobalBounds();
}







