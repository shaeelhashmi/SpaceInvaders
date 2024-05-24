#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include<ctime>
using namespace std;
using namespace sf;

class Picture {
    public:
    Picture(string filePath) {
        texture.loadFromFile(filePath);
        sprite.setTexture(texture);
    }
    void SetTexture(string filePath) {
        texture.loadFromFile(filePath);
        sprite.setTexture(texture);
    }
    void setScale(sf::Vector2f scale) {
        scale.x = std::max(scale.x, 0.1f); // Minimum scale to avoid division by zero
        scale.y = std::max(scale.y, 0.1f);
        // Calculate aspect ratio
        sf::Vector2f Aspectratio(scale.x / texture.getSize().x, scale.y / texture.getSize().y);

        // Set scale of sprite
        sprite.setScale(Aspectratio);
    }
    void setPosition(sf::Vector2f position) {
        sprite.setPosition(position);
    }
    void drawTo(sf::RenderWindow& window) {
        window.draw(sprite);
    }
    Vector2f getPosition() {
        return sprite.getPosition();
    }
    Vector2f getSize() {
        return sprite.getScale();
    }
    Vector2f getsizeText() {
        return Vector2f(texture.getSize().x, texture.getSize().y);
    }
    void move(double x, double y) {
        sprite.move(x, y);
    }
    FloatRect getGlobalBounds() {
        return sprite.getGlobalBounds();
    }

    private:
    sf::Texture texture;
    sf::Sprite sprite;
};

class Spaceship {
    Picture spaceship;

    public:
    Spaceship(RenderWindow& window) : spaceship("Spaceship.png") {
        spaceship.setScale(Vector2f(window.getSize().x * 0.082, window.getSize().y * 0.134));
        spaceship.setPosition(Vector2f((window.getSize().x / 2) - 100, window.getSize().y - 115));
    }
    bool checkleft(double move) {
        return spaceship.getPosition().x - move > 0;
    }
    bool checkright(RenderWindow& window, double move) {
        return spaceship.getPosition().x + move < window.getSize().x - 100;
    }
    bool checkUp(double move) {
        return spaceship.getPosition().y - move > 0;
    }
    bool checkdown(RenderWindow& window, double move) {
        return spaceship.getPosition().y + move < window.getSize().y - 111;
    }
    void drawTo(RenderWindow& window) {
        spaceship.drawTo(window);
    }
    Vector2f getPosition() {
        return spaceship.getPosition();
    }
    Vector2f getSize() {
        return spaceship.getSize();
    }
    void move(double x, double y) {
        spaceship.move(x, y);
    }
    FloatRect getGlobalBounds() {
        return spaceship.getGlobalBounds();
    }
};
class Asteroid {
    Picture asteroid;

    public:
    Asteroid(RenderWindow& window, string Filepath, double Corners) : asteroid(Filepath) {
        int size = (RAND_MAX % 2) + 1;

        asteroid.setScale(Vector2f(30 * size, 30 * size));
        asteroid.setPosition(Vector2f((float)rand() / RAND_MAX * window.getSize().x, -size));
        if ((asteroid.getPosition().x + Corners < window.getSize().x / 2)) {

            asteroid.move(Corners, 0);
        }
        else {
            asteroid.move(-1 * Corners, 0);
        }
    }
    void SetTexture(string filePath) {
        asteroid.SetTexture(filePath);
    }
    void move(RenderWindow& window) {
        double speed = 0.00000097 * window.getSize().y * window.getSize().x * 2;
        asteroid.move(0, speed);
    }
    void drawTo(RenderWindow& window) {
        asteroid.drawTo(window);
    }
    Vector2f getPosition() {
        return asteroid.getPosition();
    }
    Vector2f getSize() {
        return asteroid.getSize();
    }
    FloatRect getGlobalBounds() {
        return asteroid.getGlobalBounds();
    }

};
class Bullets {
    Picture bullet;

    public:
    Bullets(RenderWindow& window) : bullet("bullets.png") {
        bullet.setScale(Vector2f(window.getSize().x * 0.01239, window.getSize().y * 0.04103));
    }
    void move(RenderWindow& window) {
        float speed = 1.0f;
        bullet.move(0, -speed);
    }
    void drawTo(RenderWindow& window) {
        bullet.drawTo(window);
    }
    Vector2f getPosition() {
        return bullet.getPosition();
    }
    Vector2f getSize() {
        return bullet.getSize();
    }
    void move(double x, double y) {
        bullet.move(x, y);
    }
    FloatRect getGlobalBounds() {
        return bullet.getGlobalBounds();
    }
    void SetPosition(float x, float y) {
        bullet.setPosition(Vector2f(x, y));
    }
};
class Boss {
    Picture boss;
    Clock changeMovement;
    Clock ShootBullets;
    public:
    Boss(RenderWindow& window, string bossText) :boss(bossText) {
        boss.setScale(Vector2f(window.getSize().x * 0.082, window.getSize().y * 0.134));
        boss.setPosition(Vector2f((window.getSize().x / 2) - 100, window.getSize().y - 115));
    }
    void script(RenderWindow& window) {
        srand(time(nullptr));
        if (changeMovement.getElapsedTime().asSeconds() > 3) {
            float positionX = rand() % window.getSize().x;
            float positionY = rand() % window.getSize().y / 4;
            boss.setPosition(Vector2f(positionX, positionY));

            changeMovement.restart();
        }
        drawTo(window);
    }
    void drawTo(RenderWindow& window) {
        boss.drawTo(window);
    }
};
int main() {
    RenderWindow window(VideoMode::getDesktopMode(), "Space invader", Style::Close | Style::Fullscreen);
    window.setFramerateLimit(60);
    Spaceship spaceship(window);
    Bullets bullet(window);
    Picture hearts[5] = { Picture("hearts.png"), Picture("hearts.png"), Picture("hearts.png"), Picture("hearts.png"), Picture("hearts.png") };

    Clock clock;
    //The clock for the multiplier to end
    Clock endMultiplier;
    Clock asteroidClock;
    Boss b1(window, "Spaceship.png");
    vector<Bullets> bullets;
    vector<Asteroid> asteroids;
    //This array will store the exploded array
    vector<Asteroid> explodedAsteroids;
    //This vector will store the time for each exploded asteroid
    vector<Clock> explodedAsteroidsTime;
    //this will be increased for every asteroid destroyed

    int heart = 5;
    int multiplier = 1;
    bool a = true;
    double movement = window.getSize().x * window.getSize().y * 0.0000039 * 2;

    // hearts[0].setScale(Vector2f(40, 40));
    // hearts[1].setScale(Vector2f(40, 40));
    // hearts[2].setScale(Vector2f(40, 40));
    // hearts[3].setScale(Vector2f(40, 40));
    // hearts[4].setScale(Vector2f(40, 40));

    for (int i = 0; i < 5; i++) {
        hearts[i].setScale(Vector2f(40, 40));
    }
    for (int i = 0; i < 5; i++) {
        static int pos = 10;
        hearts[i].setPosition(Vector2f(pos, 20));
        pos += 50;
    }

    // hearts[0].setPosition(Vector2f(10, 20));
    // hearts[1].setPosition(Vector2f(60, 20));
    // hearts[2].setPosition(Vector2f(110, 20));
    // hearts[3].setPosition(Vector2f(160, 20));
    // hearts[4].setPosition(Vector2f(210, 20));

    while (window.isOpen()) {

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        if (event.key.code == Keyboard::Space && (clock.getElapsedTime().asSeconds() > 1 || a)) {
            bullet.SetPosition(spaceship.getPosition().x + (100 / 2) - (bullet.getSize().x / 2) - 10, spaceship.getPosition().y - 10);
            bullets.push_back(bullet);
            bullet.drawTo(window);
            window.display();
            clock.restart();
            a = false;
            event.key.code = Keyboard::Unknown;
        }
        if ((Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) && spaceship.checkleft(movement)) {
            spaceship.move(-1 * movement, 0);
        }
        if ((Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) && spaceship.checkright(window, movement)) {

            spaceship.move(movement, 0);
        }
        if ((Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) && spaceship.checkUp(movement)) {
            spaceship.move(0, -1 * movement);
        }
        if ((Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) && spaceship.checkdown(window, movement)) {
            spaceship.move(0, movement);
        }
        //This condition will check if the multiplier has ended
        if (endMultiplier.getElapsedTime().asSeconds() > 3) {
            multiplier = 1;
        }
        window.clear();
        for (int i = 0; i < bullets.size(); i++) {
            if (bullets[i].getPosition().y < 0) {
                bullets.erase(bullets.begin() + i);
            }
            else {
                bullets[i].move(0, window.getSize().y * -0.00133 * 2);
                bullets[i].drawTo(window);
            }
        }
        for (int i = 0; i < asteroids.size(); i++) {
            asteroids[i].SetTexture("Asteroid.png");
            asteroids[i].move(window);
            asteroids[i].drawTo(window);
            if (spaceship.getGlobalBounds().intersects(asteroids[i].getGlobalBounds())) {
                heart--;
                asteroids.erase(asteroids.begin() + i);

            }
            for (int j = 0; j < bullets.size(); j++) {
                if (asteroids[i].getGlobalBounds().intersects(bullets[j].getGlobalBounds())) {
                    Clock c;
                    asteroids[i].SetTexture("AsteroidDestructions.png");
                    explodedAsteroids.push_back(asteroids[i]);
                    explodedAsteroidsTime.push_back(c);
                    asteroids.erase(asteroids.begin() + i);
                    bullets.erase(bullets.begin() + j);
                    multiplier++;
                    endMultiplier.restart();
                    std::cout << "Multiplier: " << multiplier << endl;
                }
            }
        }
        if (asteroidClock.getElapsedTime().asSeconds() > 3.0f) {
            Asteroid as(window, "Asteroid.png", spaceship.getSize().x);
            asteroids.push_back(as);
            asteroidClock.restart();
        }
        for (int i = 0;i < explodedAsteroids.size();i++) {
            if (explodedAsteroidsTime[i].getElapsedTime().asSeconds() > 1) {
                explodedAsteroids.erase(explodedAsteroids.begin() + i);
                explodedAsteroidsTime.erase(explodedAsteroidsTime.begin() + i);
            }
            else {
                explodedAsteroids[i].SetTexture("AsteroidDestructions.png");
                explodedAsteroids[i].drawTo(window);
            }
        }
        spaceship.drawTo(window);

        for (int i = 0; i < heart; i++) {
            hearts[i].drawTo(window);
        }
        if (heart == 0) {
            cout << "GAME OVER";
            window.close();
        }
        b1.script(window);
        window.display();
    }
    return 0;
}
