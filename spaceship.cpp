
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace sf;

class Picture
{

public:
    Picture(std::string filePath)
    {
        texture.loadFromFile(filePath);
        sprite.setTexture(texture);
    }

    void setScale(sf::Vector2f scale)
    {

        sf::Vector2f Aspectratio(scale.x / texture.getSize().x, scale.y / texture.getSize().y);
        sprite.setScale(Aspectratio);
    }
    void setPosition(sf::Vector2f position)
    {

        sprite.setPosition(position);
    }
    void drawTo(sf::RenderWindow &window)
    {

        window.draw(sprite);
    }
    Vector2f getPosition() {
        return sprite.getPosition();
    }
    Vector2f getSize() {
        return sprite.getScale();
    }
    
    void move(double x, double y) {
        sprite.move(x,y);
    }
    FloatRect getGlobalBounds() {
        return sprite.getGlobalBounds();
    }
    
private:
    sf::Texture texture;
    sf::Sprite sprite;
};

class Spaceship
{   
    Picture spaceship;
public:
    Spaceship(RenderWindow& window): spaceship("R (1).png")
    {
        spaceship.setScale(Vector2f(window.getSize().x * 0.082, window.getSize().y * 0.134));
        spaceship.setPosition(Vector2f((window.getSize().x / 2) - 50, window.getSize().y - 100));
    }
    bool checkleft(double move)
    {
        return spaceship.getPosition().x - move > 0;
    }
    bool checkright(RenderWindow &window, double move)
    {
        return spaceship.getPosition().x + move < window.getSize().x - 100;
    }
    bool checkUp(double move)
    {
        return spaceship.getPosition().y - move > 0;
    }
    bool checkdown(RenderWindow &window, double move)
    {
        return spaceship.getPosition().y + move < window.getSize().y - 100;
    }
    void drawTo(RenderWindow &window) {
        spaceship.drawTo(window);
    }
    Vector2f getPosition() {
        return spaceship.getPosition();
    }
    Vector2f getSize() {
        return spaceship.getSize();
    }
    void move(double x, double y) {
        spaceship.move(x,y);
    }
    FloatRect getGlobalBounds() {
        return spaceship.getGlobalBounds();
    }
};

class Asteroid
{
    Picture asteroid;

public:
    Asteroid(RenderWindow& window): asteroid("R (2).png")
    {
        float size = (float)rand() / RAND_MAX * 20 + 10;
        asteroid.setScale(Vector2f(size, size));
        asteroid.setPosition(Vector2f((float)rand() / RAND_MAX * window.getSize().x, -size));
    }
    void move(RenderWindow& window)
    {
        float speed = 1.0f;
        asteroid.move(0, speed);
        if (asteroid.getPosition().y > window.getSize().y)
        {
            float size = (float)rand() / RAND_MAX * 20 + 10;
            asteroid.setScale(Vector2f(size, size));
            asteroid.setPosition(Vector2f((float)rand() / RAND_MAX * window.getSize().x, -size));
        }
    }
    void drawTo(RenderWindow &window) {
        asteroid.drawTo(window);
    }
    Vector2f getPosition() {
        return asteroid.getPosition();
    }
    Vector2f getSize() {
        return asteroid.getSize();
    }
    // void move(double x, double y) {
    //     asteroid.move(x,y);
    // }
    FloatRect getGlobalBounds() {
        return asteroid.getGlobalBounds();
    }
};

int main()
{
    RenderWindow window(VideoMode::getDesktopMode(), "Space invader", Style::Close | Style::Fullscreen);
    Spaceship spaceship(window);
    Clock clock;
    Vector2f bulletSize = Vector2f(window.getSize().x * 0.01239, window.getSize().y * 0.04103);
    RectangleShape bullet(bulletSize);
    bullet.setFillColor(Color::Red);
    double movement = window.getSize().x * window.getSize().y * 0.0000039;
    vector<RectangleShape> bullets;
    vector<Asteroid> asteroids;
    Clock asteroidClock;
    bool a = true;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }
            if (event.key.code == Keyboard::Space && (clock.getElapsedTime().asSeconds() > 1 || a))
            {
                bullet.setPosition(spaceship.getPosition().x + (spaceship.getSize().x / 2) - (bullet.getSize().x / 2), spaceship.getPosition().y);
                bullets.push_back(bullet);
                window.draw(bullet);
                window.display();
                clock.restart();
                a = false;
            }
        else if (Keyboard::isKeyPressed(Keyboard::A) && spaceship.checkleft(movement))
        {
            spaceship.move(-1 * movement, 0);
        }
        else if (Keyboard::isKeyPressed(Keyboard::D) && spaceship.checkright(window, movement))
        {
            spaceship.move(movement, 0);
        }
        else if (Keyboard::isKeyPressed(Keyboard::W) && spaceship.checkUp(movement))
        {
            spaceship.move(0, -1 * movement);
        }
        else if (Keyboard::isKeyPressed(Keyboard::S) && spaceship.checkdown(window, movement))
        {
            spaceship.move(0, movement);
        }
        window.clear();
        for (int i = 0; i < bullets.size(); i++)
        {
            if (bullets[i].getPosition().y < 0)
            {
                bullets.erase(bullets.begin() + i);
            }
            bullets[i].move(0, window.getSize().y * -0.00133);
            window.draw(bullets[i]);
        }
        for (int i = 0; i < asteroids.size(); i++)
        {
            asteroids[i].move(window);
            asteroids[i].drawTo(window);

            if (spaceship.getGlobalBounds().intersects(asteroids[i].getGlobalBounds()))
            {
                cout << "Game Over!" << endl;
                window.close();
            }

            if (asteroids[i].getPosition().y > window.getSize().y)
            {
                asteroids.erase(asteroids.begin() + i);
            }

            for (int j = 0; j < bullets.size(); j++) {
                if (asteroids[i].getGlobalBounds().intersects(bullets[j].getGlobalBounds())) {
                    asteroids.erase(asteroids.begin() + i);
                    bullets.erase(bullets.begin() + j);
                }
            }
        }

        if (asteroidClock.getElapsedTime().asSeconds() > 3.0f)
        {
            Asteroid as(window);
            asteroids.push_back(as);
            asteroidClock.restart();
        }

        spaceship.drawTo(window);
        window.display();
    }
    return 0;
}
