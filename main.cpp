#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream> 
#include <math.h>
#include <TileMap.hpp>
#include <Player.hpp>
#include <LetterBox.hpp>

const int TILE_SIZE = 32;
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 15;

//MESS ARROUND WITH THESE!!!!!!!!!!!
const float gravity = 0.0003f;
const float maxVelocityY = -0.25f;
const float initialVelocityY = -0.005;

//const float initialVelocityY = -0.01;
//const float gravity = 0.0003f;
//const float maxVelocityY = -0.25f;
const float accelerationX = 0.0001f;
const float maxVelocityX = 0.2f;

float velocityY = 0.0f; 
float velocityX = 0.0f;

bool isJumping = false;
bool isFalling = false;
bool jumpReleased = true; //check if W has been released to prevent holding W and instant jumping


int main()
{
    //sf::RenderWindow window(sf::VideoMode(MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE), "Tile Map");  //normal - no letterbox
    //sf::RenderWindow window(sf::VideoMode(1280,720), "Tile Map",sf::Style::Fullscreen);     //fullscreen
    sf::RenderWindow window(sf::VideoMode(MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE), "Tile Map LetterBox",(sf::Style::Resize + sf::Style::Close));
    sf::View view;
    view.setSize( MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE );
    view.setCenter( view.getSize().x / 2, view.getSize().y / 2 );
    view = getLetterboxView( view,  MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE ); 

    Player player = Player();
    TileMap tiles = TileMap(MAP_WIDTH, MAP_HEIGHT, TILE_SIZE);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;

        float deltaTime = clock.restart().asSeconds();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::Resized)
                view = getLetterboxView( view, event.size.width, event.size.height );
        }
        
        
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            jumpReleased = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !isJumping && !isFalling && jumpReleased)
        {
            isJumping = true;
            velocityY = initialVelocityY;  //initial velocity
            jumpReleased = false;
        }
        if (isJumping)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && velocityY >= maxVelocityY)
            {
                velocityY -= gravity;
            }
            else
            {
                isJumping = false;
                isFalling = true;
            }
        }else
        {   
            if(isFalling){
                velocityY += gravity;
            }
        }

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
        sf::FloatRect playerBoundsX = player.getGlobalBounds();
        sf::FloatRect playerBoundsY = player.getGlobalBounds();
        playerBoundsX.left += velocityX;
        playerBoundsY.top += velocityY;

        isFalling = true; //assume player is falling, if it's on ground, then set it to not falling
        bool doBounce = false;  //bouncing off wals

        for(sf::FloatRect tilebound: tiles.tileBounds){
            if (playerBoundsX.intersects(tilebound)){
                doBounce = true;
            }
            if (playerBoundsY.intersects(tilebound)){
                if(velocityY>=0){
                    isFalling = false;
                    velocityY = 0.0;
                }else{
                    isFalling = true;
                    isJumping = false;
                    velocityY = velocityY/2;   //hitting ceiling lowers velocity
                }
                velocityY = 0.0;
            }
        }
        
        if(doBounce){
            velocityX = -velocityX/1.25;
        }

        //std::cout<<velocityX<<'\n';
        
        //Frame rate buffer
        //dx = dx*deltaTime*1000;
        //dy = dy*deltaTime*1000;
        
        player.move(velocityX,velocityY);

        window.clear(sf::Color(255, 255, 255));
        window.setView(view);

        tiles.draw(window); 
        player.draw(window);
        window.display();
    }

    return 0;
}



