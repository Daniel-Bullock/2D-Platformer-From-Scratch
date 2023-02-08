#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream> 

const int WIDTH = 1920;
const int HEIGHT = 1080;

bool isJumping = false;
bool isFalling = false;
const float jumpHeight = 300.0f;
const float jumpDuration = 0.6f;
float jumpTime = 0.0f;
const float gravity = 1.3f;
float velocity = 0.0f;


int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "2D Platformer");

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("Textures/Player/player.png")){
        return 1;
    }
    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
    playerSprite.setPosition(WIDTH / 2, window.getSize().y - playerSprite.getGlobalBounds().height);

    // Create platform
    sf::RectangleShape platform;
    platform.setSize(sf::Vector2f(600.0f, 20.0f));
    platform.setFillColor(sf::Color(255, 200, 130));
    platform.setPosition(WIDTH - platform.getGlobalBounds().width - 50.0f, window.getSize().y - platform.getGlobalBounds().height - 70.0f);

    sf::Clock clock;
    
    while (window.isOpen())
    {   
        //Clock for jumping
        float deltaTime = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Update the player sprite based on input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !isJumping && !isFalling)
        {
            isJumping = true;
        }
        /*
        if (isJumping)
        {
            jumpTime += deltaTime;
            float y = jumpHeight * std::sin(jumpTime * (3.14159f / jumpDuration));
            playerSprite.setPosition(playerSprite.getPosition().x, window.getSize().y - playerSprite.getGlobalBounds().height - y);

            if (jumpTime >= jumpDuration)
            {
                isJumping = false;
                jumpTime = 0.0f;
            }
        }*/
        if (isJumping)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && jumpTime < jumpDuration/2)
            {
                jumpTime += deltaTime;
                float y = jumpHeight * std::sin(jumpTime * (3.14159f / jumpDuration));
                playerSprite.setPosition(playerSprite.getPosition().x, window.getSize().y - playerSprite.getGlobalBounds().height - y);
            }
            else
            {
                isJumping = false;
                isFalling = true;
                jumpTime = 0.0f;
                velocity = 0;
            }
        }else
        {
            if (playerSprite.getPosition().y >= window.getSize().y - playerSprite.getGlobalBounds().height)
            {
                playerSprite.setPosition(playerSprite.getPosition().x, window.getSize().y - playerSprite.getGlobalBounds().height);
                velocity = 0;
                isJumping = false;
                isFalling = false;
                jumpTime = 0.0f;
            }else
            {
                velocity += gravity * deltaTime;
                playerSprite.move(0, velocity);
            }
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            if (playerSprite.getPosition().x > 0)
            {
                playerSprite.move(-.2, 0);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            if (playerSprite.getPosition().x < window.getSize().x - playerSprite.getGlobalBounds().width)
            {
                playerSprite.move(.2, 0);
            }
        }

        if (playerSprite.getGlobalBounds().intersects(platform.getGlobalBounds()))
        {
            // Calculate the overlap between the player sprite and platform rectangles
            float overlap = playerSprite.getGlobalBounds().top + playerSprite.getGlobalBounds().height - platform.getGlobalBounds().top;
            // Move the player sprite up by the amount of overlap
            playerSprite.move(0, -overlap);
            // Set the velocity to 0
            velocity = 0;
            // Update the jumping and falling flags
            isJumping = false;
            isFalling = false;
            jumpTime = 0.0f;
        }

        window.clear(sf::Color(255, 255, 255));
        window.draw(playerSprite);
        window.draw(platform);
        window.display();
    }

    return 0;
}