#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream> 
#include <math.h>
#include <sstream>
#include <iomanip> 
#include <chrono>

#include <TileMap.hpp>
#include <Player.hpp>
#include <LetterBox.hpp>
#include <UI.hpp>

const int TILE_SIZE = 16;
const int MAP_WIDTH = 40;

const int MAP_HEIGHT = 28;

//Frame duration for ensuring frame independence
constexpr std::chrono::microseconds FRAME_DURATION(300);

int main()
{
    //sf::RenderWindow window(sf::VideoMode(640,360), "Tile Map");  //normal - no letterbox
    //sf::RenderWindow window(sf::VideoMode(640,360), "Tile Map",sf::Style::Fullscreen);
    sf::RenderWindow window(sf::VideoMode(640, 360), "Tile Map LetterBox",(sf::Style::Resize + sf::Style::Close));

    //window.setFramerateLimit(30);
    
    sf::View view;
    view.setSize(640, 360);
    view.setCenter( view.getSize().x / 2, view.getSize().y / 2 );
    view = getLetterboxView( view, 640, 360); 

    Player player = Player();
    TileMap tiles = TileMap(MAP_WIDTH, MAP_HEIGHT, TILE_SIZE);
 
    sf::Clock clock;

    sf::Font font;
    if (!font.loadFromFile("Textures/fibberish.ttf"))
    {
        std::cout<<"Error: couldn't load font\n";
    }

    sf::Text percentText;
    percentText.setFont(font);
    percentText.setString("");
    percentText.setCharacterSize(16); // in pixels, not points
    percentText.setFillColor(sf::Color(0, 0, 0));

    sf::Text timeText;
    timeText.setFont(font);
    timeText.setString("");
    timeText.setCharacterSize(16); // in pixels, not points
    timeText.setFillColor(sf::Color(0, 0, 0));

    //frame independence
    std::chrono::microseconds lag(0);
	std::chrono::steady_clock::time_point previousTime;
    previousTime = std::chrono::steady_clock::now();

    while (window.isOpen())
    {
        std::chrono::microseconds deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previousTime);
        lag += deltaTime;
		previousTime += deltaTime;
        
        sf::Time timeElapsed = clock.getElapsedTime();
        while (FRAME_DURATION <= lag) //frame independence
		{
            lag -= FRAME_DURATION; 

            sf::Event event;

            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event::Resized)
                    view = getLetterboxView( view, event.size.width, event.size.height );
            }

            player.update(window, tiles);
            player.move();
  
            view.setCenter(view.getCenter().x, linearInterpolation(player, view));
                
            percentText.setString(getPercentText(player.getPosition().y));

            timeText.setString(getTimeText(timeElapsed));

            //Frames are independent of fps; now draw on screen
            if (FRAME_DURATION > lag)
			{
                window.clear(sf::Color(255, 255, 255));
                
                window.setView(view);
                
                tiles.draw(window); 
                player.draw(window);

                percentText.setPosition(view.getCenter().x - 320,view.getCenter().y - 180);
                window.draw(percentText);

                timeText.setPosition(view.getCenter().x - 320,view.getCenter().y - 180 + 10);
                window.draw(timeText);

                std::cout<<player.velocityY<<'\n';

                window.display();
            }
        }
    }

    return 0;
}

