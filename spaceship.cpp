
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace sf;

class Picture
{
public:
    Picture(string filePath)
    {
        texture.loadFromFile(filePath);
        sprite.setTexture(texture); 
    }
    void SetTexture(string filePath)
    {
        texture.loadFromFile(filePath);
        sprite.setTexture(texture);
    }
    void setscale(sf::Vector2f scale)
    {
       scale.x = std::max(scale.x, 0.1f); // Minimum scale to avoid division by zero
    scale.y = std::max(scale.y, 0.1f);
  // Calculate aspect ratio
    sf::Vector2f Aspectratio(scale.x / texture.getSize().x, scale.y / texture.getSize().y);

    // Set scale of sprite
    sprite.setScale(Aspectratio);
    }
    void setposition(sf::Vector2f position)
    {
        sprite.setPosition(position);
    }
    void drawTo(sf::RenderWindow &window)
    {
        window.draw(sprite);
    }
    Vector2f getposition() {
        return sprite.getPosition();
    }
    Vector2f getSize() {    
        return sprite.getScale();
    }
    Vector2f getsizeText(){
        return Vector2f(texture.getSize().x, texture.getSize().y);
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
    Spaceship(RenderWindow& window): spaceship("Spaceship.png")
    {
        spaceship.setscale(Vector2f(window.getSize().x * 0.082, window.getSize().y * 0.134));
        spaceship.setposition(Vector2f((window.getSize().x / 2) - 100, window.getSize().y - 111));
    }
    bool checkleft(double move)
    {
        return spaceship.getposition().x - move > 0;
    }
    bool checkright(RenderWindow &window, double move)
    {
        return spaceship.getposition().x + move < window.getSize().x - 100;
    }
    bool checkUp(double move)
    {
        return spaceship.getposition().y - move > 0;
    }
    bool checkdown(RenderWindow &window, double move)
    {
        return spaceship.getposition().y + move < window.getSize().y - 111;
    }
    void drawTo(RenderWindow &window) {
        spaceship.drawTo(window);
    }
    Vector2f getPosition() {
        return spaceship.getposition();
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
    Asteroid(RenderWindow& window,string Filepath): asteroid(Filepath)
    {
        float size = (float)rand() / RAND_MAX * 20 + 10;
        asteroid.setscale(Vector2f(size, size));
        asteroid.setposition(Vector2f((float)rand() / RAND_MAX * window.getSize().x, -size));
    }
    void SetTexture(string filePath)
    {
        asteroid.SetTexture(filePath);
    }
    void move(RenderWindow& window)
    {
        float speed = 1.0f;
        asteroid.move(0, speed);
    }
    void drawTo(RenderWindow &window) {
        asteroid.drawTo(window);
    }
    Vector2f getPosition() {
        return asteroid.getposition();
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
class Bullets
{
    Picture bullet;
public: 
    Bullets(RenderWindow& window): bullet("Bullets.png")
    {
        bullet.setscale(Vector2f(window.getSize().x * 0.01239*2, window.getSize().y * 0.04103));
    }
    void move(RenderWindow& window)
    {
        float speed = 1.0f;
        bullet.move(0, -speed);
    }
    void drawTo(RenderWindow &window) {
        bullet.drawTo(window);
    }
    Vector2f getPosition() {
        return bullet.getposition();
    }
    Vector2f getSize() {
        return bullet.getSize();
    }
    void move(double x, double y) {
        bullet.move(x,y);
    }
    FloatRect getGlobalBounds() {
        return bullet.getGlobalBounds();
    }
    void SetPosition(float x, float y) {
        bullet.setposition(Vector2f(x, y));
    }
};
int main()
{
    RenderWindow window(VideoMode::getDesktopMode(), "Space invader", Style::Close | Style::Fullscreen);
    Spaceship spaceship(window);
    Clock clock;
    Bullets bullet(window);
    double movement = window.getSize().x * window.getSize().y * 0.0000039;
    vector<Bullets> bullets;
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
                bullet.SetPosition(spaceship.getPosition().x + (100/ 2) - (bullet.getSize().x / 2)-10, spaceship.getPosition().y-10);
                bullets.push_back(bullet);
                bullet.drawTo(window);
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
            else
            {
bullets[i].move(0, window.getSize().y * -0.00133);
            bullets[i].drawTo(window);
            }
            
        }
        for (int i = 0; i < asteroids.size(); i++)
        {
            asteroids[i].drawTo(window);
            asteroids[i].move(window);
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
            Asteroid as(window,"Asteroid.png");
            asteroids.push_back(as);
            asteroids[asteroids.size() - 1].SetTexture("Asteroid.png");
            asteroidClock.restart();
        }
    spaceship.drawTo(window);
    window.display();
    }
    return 0;
}