#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace sf;

class Spaceship
{
    RectangleShape spaceship;

public:
    Spaceship(RenderWindow& window)
    {
        spaceship.setSize(Vector2f(window.getSize().x * 0.082, window.getSize().y * 0.134));
        spaceship.setFillColor(Color::Green);
        spaceship.setPosition((window.getSize().x / 2) - 50, window.getSize().y - 100);
    }
    RectangleShape getSpaceship()
    {
        return spaceship;
    }
    bool checkleft(RectangleShape& ship, double move)
    {
        return ship.getPosition().x - move > 0;
    }
    bool checkright(RectangleShape& ship, RenderWindow& window, double move)
    {
        return ship.getPosition().x + move < window.getSize().x - 100;
    }
    bool checkUp(RectangleShape& ship, double move)
    {
        return ship.getPosition().y - move > 0;
    }
    bool checkdown(RectangleShape& ship, RenderWindow& window, double move)
    {
        return ship.getPosition().y + move < window.getSize().y - 100;
    }
};

class Asteroid
{
    RectangleShape asteroid;

public:
    Asteroid(RenderWindow& window)
    {
        float size = (float)rand() / RAND_MAX * 20 + 10;
        asteroid.setSize(Vector2f(size, size));
        asteroid.setFillColor(Color::White);
        asteroid.setPosition((float)rand() / RAND_MAX * window.getSize().x, -size);
    }
    RectangleShape getAsteroid()
    {
        return asteroid;
    }
    void move(RenderWindow& window)
    {
        float speed = 1.0f;
        asteroid.move(0, speed);
        if (asteroid.getPosition().y > window.getSize().y)
        {
            float size = (float)rand() / RAND_MAX * 20 + 10;
            asteroid.setSize(Vector2f(size, size));
            asteroid.setPosition((float)rand() / RAND_MAX * window.getSize().x, -size);
        }
    }
};

int main()
{
    RenderWindow window(VideoMode::getDesktopMode(), "Space invader", Style::Close | Style::Fullscreen);
    Spaceship s(window);
    Clock clock;
    Vector2f bulletSize = Vector2f(window.getSize().x * 0.01239, window.getSize().y * 0.04103);
    RectangleShape bullet(bulletSize);
    bullet.setFillColor(Color::Red);
    double movement = window.getSize().x * window.getSize().y * 0.0000039;
    RectangleShape spaceship = s.getSpaceship();
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
        if (event.type == Event::MouseButtonPressed && (clock.getElapsedTime().asSeconds() > 1 || a))
        {
            if (event.key.code == Mouse::Left)
            {
                bullet.setPosition(spaceship.getPosition().x + (spaceship.getSize().x / 2) - (bullet.getSize().x / 2), spaceship.getPosition().y);
                bullets.push_back(bullet);
                window.draw(bullet);
                window.display();
                clock.restart();
                a = false;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::A) && s.checkleft(spaceship, movement))
        {
            spaceship.move(-1 * movement, 0);
        }
        else if (Keyboard::isKeyPressed(Keyboard::D) && s.checkright(spaceship, window, movement))
        {
            spaceship.move(movement, 0);
        }
        else if (Keyboard::isKeyPressed(Keyboard::W) && s.checkUp(spaceship, movement))
        {
            spaceship.move(0, -1 * movement);
        }
        else if (Keyboard::isKeyPressed(Keyboard::S) && s.checkdown(spaceship, window, movement))
        {
            spaceship.move(0, movement);
        }
        window.clear();
        window.draw(spaceship);
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
            window.draw(asteroids[i].getAsteroid());

            if (spaceship.getGlobalBounds().intersects(asteroids[i].getAsteroid().getGlobalBounds()))
            {
                cout << "Game Over!" << endl;
                window.close();
            }

            if (asteroids[i].getAsteroid().getPosition().y > window.getSize().y)
            {
                asteroids.erase(asteroids.begin() + i);
            }

            for (int j = 0; j < bullets.size(); j++) {
                if (asteroids[i].getAsteroid().getGlobalBounds().intersects(bullets[j].getGlobalBounds())) {
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

        window.display();
    }
    return 0;
}
