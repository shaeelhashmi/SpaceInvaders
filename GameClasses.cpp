#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
using namespace sf;
class Button {
    public:
    Button(string t, Vector2f size, int charSize, Color bgColor, Color textColor, float offset = 1)
        : offset(offset) {
        text.setString(t);
        text.setFillColor(textColor);
        text.setCharacterSize(charSize);
        button.setSize(size);
        button.setFillColor(bgColor);
    }
    void setFont(Font& font) {
        text.setFont(font);
    }
    void setBackColor(Color color) {
        button.setFillColor(color);
    }
    void setTextColor(Color color) {
        text.setFillColor(color);
    }
    void setPosition(Vector2f pos) {
        button.setPosition(pos);
        float xPos = pos.x + (button.getLocalBounds().width - text.getLocalBounds().width) / 2;
        float yPos = pos.y + (button.getLocalBounds().height - text.getLocalBounds().height) / 2;
        yPos -= offset;
        text.setPosition(xPos, yPos);
    }
    Vector2f getPosition() const {
        return button.getPosition();
    }
    Vector2f getSize() const {
        return button.getSize();
    }
    void drawTo(RenderWindow& window) {
        window.draw(button);
        window.draw(text);
    }
    bool buttonClicked(RenderWindow& window) {
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

        return button.getGlobalBounds().contains(mousePos);
    }
    void leftalign(Vector2f pos, float textOffset) {
        button.setPosition(pos);
        float xPos = pos.x + textOffset;
        float yPos = (pos.y + button.getLocalBounds().height / 2) - (text.getLocalBounds().height / 2);
        text.setPosition(xPos, yPos);
    }
    Text getText() {
        return text;
    }
    private:
    RectangleShape button;
    Text text;
    float offset;
};
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
    void rotate(float angle) {
        sprite.rotate(angle);
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
    void setPosition(Vector2f position) {
        spaceship.setPosition(position);
    }
    bool checkleft(double move) {
        return spaceship.getPosition().x - move > 0;
    }
    bool checkright(RenderWindow& window, double move) {
        return spaceship.getPosition().x + move < window.getSize().x - 100;
    }
    bool checkUp(double move, RenderWindow& window) {
        return spaceship.getPosition().y - move > window.getSize().y / 2;
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
    int size;


    public:
    static bool generated;
    Asteroid(RenderWindow& window, string Filepath, double Corners) : asteroid(Filepath) {
        if (generated) {
            srand(time(nullptr));
            generated = false;
        }
        size = (rand() % 2) + 1;
        asteroid.setScale(Vector2f(50 * size, 50 * size));
        asteroid.setPosition(Vector2f(rand() % window.getSize().x - 100, 0));
        if ((asteroid.getPosition().x + Corners < window.getSize().x / 2)) {
            asteroid.move(Corners, 0);
        }
        else {
            asteroid.move(-1 * Corners, 0);
        }
    }
    void setRandomPosition(RenderWindow& window) {
        asteroid.setPosition(Vector2f(rand() % window.getSize().x - 100, 0));
    }
    void setRandomSize() {
        size = (rand() % 2) + 1;
        asteroid.setScale(Vector2f(50 * size, 50 * size));
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
    int getSizeValue() {
        return size;
    }
};
bool Asteroid::generated = true;
class Bullets {
    Picture bullet;

    public:
    Bullets(RenderWindow& window, string Texture) : bullet(Texture) {
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
    int hits;
    bool generated;
    public:
    Boss(RenderWindow& window, string bossText) : boss(bossText) {
        boss.setScale(Vector2f(window.getSize().x * 0.082, window.getSize().y * 0.134));
        boss.setPosition(Vector2f((window.getSize().x / 2) - 100, 0));
        hits = 0;
        generated = true;
    }

    void script(RenderWindow& window, Spaceship& spaceship, int TeleportationTime) {
        if (generated) {
            srand(time(nullptr));
            generated = false;
        }
        if (changeMovement.getElapsedTime().asSeconds() > TeleportationTime) {
            float positionX = spaceship.getPosition().x;
            float positionY = (rand() % window.getSize().y / 4) + 100;
            boss.setPosition(Vector2f(positionX, positionY));
            changeMovement.restart();
        }
        drawTo(window);

    }
    void incrementHits() {
        hits++;
    }
    void drawTo(RenderWindow& window) {
        boss.drawTo(window);
    }
    Vector2f getPosition() {
        return boss.getPosition();
    }
    FloatRect getGlobalBounds() {
        return boss.getGlobalBounds();
    }
    void Follow(Spaceship& spaceship, double movement) {
        int spaceshipPosX = spaceship.getPosition().x;
        int bossPosX = boss.getPosition().x;
        if (spaceshipPosX > bossPosX) {
            boss.move(movement, 0);
        }
        else if (spaceshipPosX < bossPosX) {
            boss.move(-movement, 0);
        }
    }
    int getHits() {
        return hits;
    }
};
class AirStrikeMinion {
    Picture minion;
    public:
    AirStrikeMinion(RenderWindow& window, string minionText) :minion(minionText) {
        minion.setScale(Vector2f(window.getSize().x * 0.082, window.getSize().y * 0.009));
        minion.setPosition(Vector2f(window.getSize().x - 100, 100));
    }
    void setStartingPosition(RenderWindow& window) {
        minion.setPosition(Vector2f(window.getSize().x - 100, 100));
    }
    void script(RenderWindow& window) {
        minion.move(-4, 0);
        drawTo(window);
    }
    void drawTo(RenderWindow& window) {
        minion.drawTo(window);
    }
    Vector2f getPosition() {
        return minion.getPosition();
    }
    void setPosition(Vector2f position) {
        minion.setPosition(position);
    }
};

class SpecialAsteroid {
    Picture specialast;

public:
    SpecialAsteroid(RenderWindow& window, string specialtex) : specialast(specialtex) {
        specialast.setScale(Vector2f(50, 50));
        specialast.setPosition(Vector2f(rand() % window.getSize().x-100, 0));
    }
    void setRandomPosition(RenderWindow& window) {
        specialast.setPosition(Vector2f(rand() % window.getSize().x-100, 0));
    }
    void move(RenderWindow& window) {
        float speed = 3.5f;
        specialast.move(0, speed);
    }
    void drawTo(RenderWindow& window) {
        specialast.drawTo(window);
    }
    Vector2f getPosition() {
        return specialast.getPosition();
    }
    Vector2f getSize() {
        return specialast.getSize();
    }
    FloatRect getGlobalBounds() {
        return specialast.getGlobalBounds();
    }
};
