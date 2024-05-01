#include <SFML/Graphics.hpp>
#include<iostream>
#include<vector>
using namespace std;
using namespace sf;
//class containing all the properties of the spaceship
class Spaceship{
    RectangleShape spaceship;
    public:
    //Defining the size color and position of the spaceship
    Spaceship(RenderWindow &window)
    {
        spaceship.setSize(Vector2f(window.getSize().x*0.082,window.getSize().y*0.134));
        spaceship.setFillColor(Color::Green);
        spaceship.setPosition((window.getSize().x/2)-50,window.getSize().y-100);
    }
    RectangleShape getSpaceship()
    {
        return spaceship;
    }
    //Functions for checking whether the movement of the spaceship is possible or not
    bool checkleft(RectangleShape &ship,double move)
    {
        return ship.getPosition().x-move>0;
    }
     bool checkright(RectangleShape &ship,RenderWindow &window,double move)
    {
        return ship.getPosition().x+move<window.getSize().x-100;
    }
     bool checkUp(RectangleShape &ship,double move)
    {
        return ship.getPosition().y-move>0;
    }
     bool checkdown(RectangleShape &ship,RenderWindow &window,double move)
    {
        return ship.getPosition().y+move<window.getSize().y-100;
    }
    
};
class Picture
{
    Texture texture;
    Sprite sprite;
    public:
    Picture(string path,RenderWindow &window)
    {
        texture.loadFromFile(path);
        sprite.setTexture(texture);
    }
    void setScale(Vector2f scale)
    {
        Vector2f Aspectratio(scale.x/texture.getSize().x,scale.y/texture.getSize().y); 
        sprite.setScale(Aspectratio);
    }
    void setPosition(Vector2f position)
    {
        sprite.setPosition(position);
    }
    void drawTo(RenderWindow &window)
    {
        window.draw(sprite);
    }
};
int main()
{
    RenderWindow window(VideoMode::getDesktopMode(), "Space invader",Style::Close|Style::Fullscreen);
    Spaceship s(window);
    Clock clock;
    Vector2f bulletSize = Vector2f(window.getSize().x*0.01239, window.getSize().y*0.04103);
    RectangleShape bullet(bulletSize);
    bullet.setFillColor(Color::Red);
    double movement=window.getSize().x*window.getSize().y*0.0000039;
    RectangleShape spaceship=s.getSpaceship();
    vector<RectangleShape> bullets;
    bool a=true;
    while (window.isOpen())
    {
       Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }   
        if (event.type == Event::MouseButtonPressed&&(clock.getElapsedTime().asSeconds()>1||a))
        {
            if (event.key.code == Mouse::Left)
            {
                bullet.setPosition(spaceship.getPosition().x+(spaceship.getSize().x/2)-(bullet.getSize().x/2), spaceship.getPosition().y);
                bullets.push_back(bullet);
                window.draw(bullet);
                window.display();
                clock.restart();
                a=false;
            }
     
        }  
        if(Keyboard::isKeyPressed(Keyboard::Left)&&s.checkleft(spaceship,movement))
        {
         spaceship.move(-1*movement, 0);
        }
        else if(Keyboard::isKeyPressed(Keyboard::Right)&&s.checkright(spaceship,window,movement))
        {
            spaceship.move(movement, 0);
        }
        else if(Keyboard::isKeyPressed(Keyboard::Up)&&s.checkUp(spaceship,movement))
        {
            spaceship.move(0, -1*movement);
        }
        else if(Keyboard::isKeyPressed(Keyboard::Down)&&s.checkdown(spaceship,window,movement))
        {
            spaceship.move(0,movement);
        }
         window.clear();
        window.draw(spaceship);
        for(int i=0;i<bullets.size();i++)
        {
            if(bullets[i].getPosition().y<0)
            {
                bullets.erase(bullets.begin()+i);
            }
            bullets[i].move(0, window.getSize().y*-0.00133);
            window.draw(bullets[i]);
        }
        window.display();
    }
    return 0;
}
    }
    return 0;
}
