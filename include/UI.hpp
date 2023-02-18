#ifndef _UI_H_
#define _UI_H_

//#include <SFML/Graphics.hpp>


std::string getPercentText(float currY);

std::string getTimeText(sf::Time timeElapsed);

float linearInterpolation(Player& player, sf::View& view);

#endif