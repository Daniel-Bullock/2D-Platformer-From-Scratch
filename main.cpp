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
#include <Global.hpp>
#include <Enemy.hpp>

int main()
{
    //sf::RenderWindow window(sf::VideoMode(640,360), "Tile Map");  //normal - no letterbox
    //sf::RenderWindow window(sf::VideoMode(640,360), "Tile Map",sf::Style::Fullscreen);
    sf::RenderWindow window(sf::VideoMode(640, 360), "Tile Map LetterBox",(sf::Style::Resize + sf::Style::Close));
    //window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(30);
    
    sf::View view;
    view.setSize(640, 360);
    view.setCenter( view.getSize().x / 2, view.getSize().y / 2 );
    view = getLetterboxView( view, 640, 360); 

    sf::Clock clock;

    Player player = Player();

    std::vector<Enemy> enemies;
    sf::Texture enemyTexture; //load texture for enemies
    enemyTexture.loadFromFile("Textures/Player/slimetest1.png");

    //Enemy enemyTest = Enemy(2 * TILE_SIZE,2 * TILE_SIZE, 0.016, enemyTexture, TILE_SIZE);
    
    //enemies.push_back(Enemy(7 * TILE_SIZE,5 * TILE_SIZE, 0.016, enemyTexture, TILE_SIZE));

    for (Enemy currEnemy: enemies)
    {
        std::cout<<currEnemy.x<<' '<<currEnemy.y<<" "<< currEnemy.getPos().x<<"\n";
    }
    std::cout<<"ASDASD\n";
    TileMap tiles = TileMap(MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, enemies);


    /*
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            if(tiles.tilemap[i][j] == 9)
            {
                //make new enemy at this location and add to list of enemies
                enemies.push_back(Enemy(j * TILE_SIZE,i * TILE_SIZE, 0.016, enemyTexture, TILE_SIZE));
                //enemies.push_back(Enemy(j * tileSize,i * tileSize, 0.016, tileSize));
            }
        }
    }*/

    for (Enemy currEnemy: enemies)
    {
        std::cout<<currEnemy.x<<' '<<currEnemy.y<<" "<< currEnemy.getPos().x<<"\n";
    }
    
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
            

            //std::cout<<"here3\n";
            //print out
            
            int topIdx = std::max(int((view.getCenter().y - 180)/16), 0);
            int bottomIdx = std::min(int((view.getCenter().y + 180)/16) + 1, MAP_HEIGHT);
            //std::cout<<enemies[0].y<<" ";
            //for (Enemy currEnemy: enemies)
            for (int i = 0; i < enemies.size(); i++)
            {
                if (int(enemies[i].y/TILE_SIZE) >= topIdx && int(enemies[i].y/TILE_SIZE) < bottomIdx)
                {
                    enemies[i].update(tiles.tilemap, TILE_SIZE);
                }
            }
            
            percentText.setString(getPercentText(player.getPosition().y));

            timeText.setString(getTimeText(timeElapsed));
            //Frames are independent of fps; now draw on screen
            if (FRAME_DURATION > lag)
			{
                window.clear(sf::Color(255, 255, 255));
                
                window.setView(view);

                player.draw(window);
                
                tiles.draw(window, view); 
                
                /*for (Enemy currEnemy: enemies)
                {
                    //if (int(currEnemy.y/TILE_SIZE) >= topIdx && int(currEnemy.y/TILE_SIZE) < bottomIdx)
                    //{
                    currEnemy.draw(window);
                    //}
                }*/
                for (int i = 0; i < enemies.size(); i++)
                {
                    if (int(enemies[i].y/TILE_SIZE) >= topIdx && int(enemies[i].y/TILE_SIZE) < bottomIdx)
                    {
                        enemies[i].draw(window);
                    }
                    
                    //std::cout<<currEnemy.x<<"\n";
                }
                /*
                for (int x = 0; x < enemies.size(); x++)
                {
                    //if (currEnemy.y >= topIdx && currEnemy.y <= bottomIdx)
                    //{
                    //std::cout<<"here7\n";
                    //enemies[x].draw(window);
                    //}
                }
                */
                //enemyTest.draw(window);

                percentText.setPosition(view.getCenter().x - 320,view.getCenter().y - 180);
                window.draw(percentText);

                timeText.setPosition(view.getCenter().x - 320,view.getCenter().y - 180 + 10);
                window.draw(timeText);             

                window.display();
            }
        }
    }
    return 0;
}

