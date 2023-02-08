#ifndef _Player_H_
#define _Player_H_

class Player
{
public:
    Player();

    //void update(float deltaTime);
    //void update(float dx,float dy);
    void draw(sf::RenderWindow &window);
    void move(float x, float y);
    //bool collides();

    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f pos);
    sf::FloatRect getGlobalBounds();

private:
    sf::Texture playerTexture;
    sf::Sprite playerSprite;
    sf::Vector2f velocity;

    const float GRAVITY = 9.8f;
    const float JUMP_FORCE = 500.0f;
};



#endif