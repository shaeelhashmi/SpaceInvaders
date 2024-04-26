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
    Spaceship()
    {
        spaceship.setSize(Vector2f(50, 50));
        spaceship.setFillColor(Color::Green);
        spaceship.setPosition(125, 250);
    }
    RectangleShape getSpaceship()
    {
        return spaceship;
    }
    //Functions for checking whether the movement of the spaceship is possible or not
    bool checkleft(RectangleShape ship)
    {
        return ship.getPosition().x-0.1>0;
    }
     bool checkright(RectangleShape ship)
    {
        return ship.getPosition().x+0.1<250;
    }
     bool checkUp(RectangleShape ship)
    {
        return ship.getPosition().y-0.1>0;
    }
     bool checkdown(RectangleShape ship)
    {
        return ship.getPosition().y+0.1<250;
    }
    
};
int main()
{
    RenderWindow window(VideoMode(300, 300), "Space invader");
    Spaceship s;
    Clock clock;
    RectangleShape spaceship=s.getSpaceship();
    RectangleShape bullet(Vector2f(5,5));
    bullet.setFillColor(Color::Red);
    vector<RectangleShape> bullets;
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
        
        if (event.type == Event::MouseButtonPressed&&clock.getElapsedTime().asSeconds()>0.2)
        {
            if (event.key.code == Mouse::Left)
            {
                bullet.setPosition(spaceship.getPosition().x+25, spaceship.getPosition().y);
                bullets.push_back(bullet);
                window.draw(bullet);
                window.display();
                clock.restart();
            }
            
        }  
        if(Keyboard::isKeyPressed(Keyboard::Left)&&s.checkleft(spaceship))
        {
            spaceship.move(-0.4, 0);
        }
        else if(Keyboard::isKeyPressed(Keyboard::Right)&&s.checkright(spaceship))
        {
            spaceship.move(0.4, 0);
        }
        else if(Keyboard::isKeyPressed(Keyboard::Up)&&s.checkUp(spaceship))
        {
            spaceship.move(0, -0.4);
        }
        else if(Keyboard::isKeyPressed(Keyboard::Down)&&s.checkdown(spaceship))
        {
            spaceship.move(0, 0.4);
        }
         window.clear();
        window.draw(spaceship);
        for(int i=0;i<bullets.size();i++)
        {
            if(bullets[i].getPosition().y<0)
            {
                bullets.erase(bullets.begin()+i);
                cout<<bullets.size()<<endl;
            }
            bullets[i].move(0, -0.5);
            window.draw(bullets[i]);
        }
        window.display();
    }
    return 0;
}