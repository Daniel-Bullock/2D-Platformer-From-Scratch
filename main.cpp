#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream> 
#include <math.h>
#include <TileMap.hpp>
#include <Player.hpp>
#include <LetterBox.hpp>
#include <chrono>
#include <sstream>
#include <iomanip> 




//DIVIDE EVERYTHING BY TWO FOR 16x16







const int TILE_SIZE = 32;
const int MAP_WIDTH = 40;

const int MAP_HEIGHT = 28;
int topOfMap = 360; 
int bottomOfMap = 525; 

float lerpFactor = 0.001f; // set the interpolation factor (between 0 and 1)

//Frame duration for ensuring frame independence
constexpr std::chrono::microseconds FRAME_DURATION(300);

int main()
{
    //sf::RenderWindow window(sf::VideoMode(1280,720), "Tile Map");  //normal - no letterbox
    sf::RenderWindow window(sf::VideoMode(1280,720), "Tile Map",sf::Style::Fullscreen);     //fullscreen
    //sf::RenderWindow window(sf::VideoMode(640,360), "Tile Map",sf::Style::Fullscreen);

    //sf::RenderWindow window(sf::VideoMode(MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE), "Tile Map LetterBox",(sf::Style::Resize + sf::Style::Close));

    //sf::RenderWindow window(sf::VideoMode(640, 360), "Tile Map LetterBox",(sf::Style::Resize + sf::Style::Close));
    //window.setFramerateLimit(30);
    
    sf::View view;
    //view.setSize( MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE );
    view.setSize(1280, 720);
    //view.setSize(640, 360);
    view.setCenter( view.getSize().x / 2, view.getSize().y / 2 );
    //view = getLetterboxView( view, 1280, 720); 

    Player player = Player();
    TileMap tiles = TileMap(MAP_WIDTH, MAP_HEIGHT, TILE_SIZE);
 
    sf::Clock clock;

    sf::Font font;
    //if (!font.loadFromFile("Textures/AOTFShinGoProBold.otf"))
    if (!font.loadFromFile("Textures/fibberish.ttf"))
    {
        std::cout<<"Error: couldn't load font\n";
    }

    sf::Text percentText;
    percentText.setFont(font);
    percentText.setString("");
    percentText.setCharacterSize(40); // in pixels, not points
    percentText.setFillColor(sf::Color(0, 0, 0));

    sf::Text timeText;
    timeText.setFont(font);
    timeText.setString("");
    timeText.setCharacterSize(40); // in pixels, not points
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
        
        sf::Time elapsed = clock.getElapsedTime();
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

            // interpolate between the current camera position and the desired camera position
            sf::Vector2f targetPos(player.getPosition().x/2, player.getPosition().y + player.getGlobalBounds().height/2); // calculate the desired camera position
            float interpolatedY = view.getCenter().y + (targetPos.y - view.getCenter().y) * lerpFactor;

            //clamp bottom and top of map
            if(interpolatedY > bottomOfMap){
                interpolatedY = bottomOfMap;
            }else if(interpolatedY < topOfMap){
                interpolatedY = topOfMap;
            }
    
            view.setCenter(view.getCenter().x, interpolatedY);

            int bottom = 831;
            int top = 64;
            int percentUp = std::min(std::max(0, 100 - int(100 * (top-player.getPosition().y)/(top-bottom))), 100);
            std::string percentUpString = std::to_string(percentUp) + "%";
            percentText.setString(percentUpString);

            int totalSeconds = static_cast<int>(elapsed.asSeconds());
            int hours = totalSeconds / 3600;
            int minutes = (totalSeconds % 3600) / 60;
            int seconds = totalSeconds % 60;
            
            std::ostringstream timeStream;
            timeStream << std::setfill('0') << std::setw(2) << hours << ":"
                    << std::setfill('0') << std::setw(2) << minutes << ":"
                    << std::setfill('0') << std::setw(2) << seconds;
            timeText.setString(timeStream.str());

            //Frames are independence of fps; now draw on screen
            if (FRAME_DURATION > lag)
			{
                window.clear(sf::Color(255, 255, 255));
                //window.clear(sf::Color(238, 206, 162));
                
                window.setView(view);
                
                tiles.draw(window); 
                player.draw(window);

                percentText.setPosition(view.getCenter().x - 640,view.getCenter().y - 360);
                window.draw(percentText);

                timeText.setPosition(view.getCenter().x - 640,view.getCenter().y - 360 + 40);
                window.draw(timeText);

                window.display();
            }
        }
    }

    return 0;
}

