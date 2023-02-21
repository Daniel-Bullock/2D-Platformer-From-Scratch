#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream> 
#include <math.h>
#include <sstream>
#include <iomanip> 

#include "Player.hpp"
#include "TileMap.hpp"
#include "UI.hpp"
#include <Global.hpp>

float lerpFactor = 0.0005f;

int topOfMapView = 180; 
int bottomOfMapView = MAP_HEIGHT*TILE_SIZE - 180;

int topPixels = TILE_SIZE * 2;
int bottomPixels = MAP_HEIGHT*TILE_SIZE - TILE_SIZE*2;


std::string getPercentText(float currY){
    int percentUp = std::min(std::max(0, 100 - int(100 * (topPixels - currY)/(topPixels - bottomPixels))), 100);
    return std::to_string(percentUp) + "%";
}

std::string getTimeText(sf::Time timeElapsed){
    int totalSeconds = static_cast<int>(timeElapsed.asSeconds());
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;
    
    std::ostringstream timeStream;
    timeStream << std::setfill('0') << std::setw(2) << hours << ":"
            << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setfill('0') << std::setw(2) << seconds;
    return timeStream.str();
}

float linearInterpolation(Player& player, sf::View& view){
    // interpolate between the current camera position and the desired camera position
    sf::Vector2f targetPos(player.getPosition().x/2, player.getPosition().y + player.getGlobalBounds().height/2); // calculate the desired camera position
    float interpolatedY = view.getCenter().y + (targetPos.y - view.getCenter().y) * lerpFactor;

    //clamp bottom and top of map
    if(interpolatedY > bottomOfMapView){
        interpolatedY = bottomOfMapView;
    }else if(interpolatedY < topOfMapView){
        interpolatedY = topOfMapView;
    }

    return interpolatedY; 
}








