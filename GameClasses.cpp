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

    void SetTexture(string filePath) {
        spaceship.SetTexture(filePath);
        spaceship.setScale(Vector2f(50, 50));
    }
};

class Asteroid {
    Picture asteroid;
    int size;
    int health;
    int hits;
    public:
    static bool generated;
    Asteroid(RenderWindow& window, string Filepath, double Corners) : asteroid(Filepath) {
        if (generated) {
            srand(time(nullptr));
            generated = false;
        }
        size = (rand() % 2) + 1;
        health = size;
        hits=0;
        asteroid.setScale(Vector2f(50 * size, 50 * size));
        asteroid.setPosition(Vector2f(rand() % window.getSize().x - 100, 0));
        if(asteroid.getPosition().x+Corners>window.getSize().x ){
            asteroid.setPosition(Vector2f(window.getSize().x/2, 0));
        }
        else if(asteroid.getPosition().x-Corners  < window.getSize().x){
            asteroid.setPosition(Vector2f(window.getSize().x/2, 0));
        }
    }
    void setRandomPosition(RenderWindow& window) {
        asteroid.setPosition(Vector2f(rand() % window.getSize().x - 100, 0));
           if(asteroid.getPosition().x+30>=window.getSize().x ){
            asteroid.setPosition(Vector2f(asteroid.getPosition().x-30, 0));
        }
        else if(asteroid.getPosition().x-30  <=0){
            asteroid.setPosition(Vector2f(asteroid.getPosition().x+30, 0));
        }
    }
    void setRandomSize() {
        size = (rand() % 2) + 1;
        health = size;
        asteroid.setScale(Vector2f(50 * size, 50 * size));
    }
    void SetTexture(string filePath) {
        asteroid.SetTexture(filePath);
    }
    void move(RenderWindow& window, int level, string difficulty) {

        double speed = window.getSize().y * 0.005 + (level * 0.008);
        if (difficulty == "hard") {
            speed = window.getSize().y * 0.005 + (level * 0.008);
        }
        else if (difficulty == "medium") {
            speed = window.getSize().y * 0.003 + (level * 0.007);
        }
        else {
            speed = window.getSize().y * 0.001 + (level * 0.006);
        }
        asteroid.move(0, speed);
    }
    void incrementHits() {
        ++hits;
    }
    bool asteroidDestroyed() {
        return hits >= health;
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
        srand(time(nullptr));
        specialast.setPosition(Vector2f(rand() % window.getSize().x - 100, 0));
    }
    void setRandomPosition(RenderWindow& window) {
        specialast.setPosition(Vector2f(rand() % window.getSize().x - 100, 0));
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

class Revive  {
    Picture revive;
    int revives;
    Clock PowerUpTimer;
public:
    Revive(RenderWindow& window, string reviveText) : revive(reviveText) {
        revive.setScale(Vector2f(50,50));
        revive.setPosition(Vector2f(rand() % (window.getSize().x - 100), 0));
        revives=0;
    }
    void increaseRevives() {
        revives++;
    }
    void drawTo(RenderWindow& window)  {
        revive.drawTo(window);
    }

    void move(double movement)  {
        revive.move(0, movement * 2);
    }
    FloatRect getGlobalBounds()  {
        return revive.getGlobalBounds();
    }

    int timer = rand() % 80;

    void script(RenderWindow& window,Spaceship& spaceship)  {
        if(PowerUpTimer.getElapsedTime().asSeconds()> timer){
            
            revive.move(0, 2);
            revive.drawTo(window);
            if(spaceship.getGlobalBounds().intersects(revive.getGlobalBounds())){
                increaseRevives();
                revive.setPosition(Vector2f(rand() % (window.getSize().x - 100), 0));
                PowerUpTimer.restart();        
            }
            else if(revive.getPosition().y>window.getSize().y){
                PowerUpTimer.restart();
                 revive.setPosition(Vector2f(rand() % (window.getSize().x - 100), 0));
            }

        }
        
    }
    void SetHealth(int &hearts) {
        if(revives>0){
            hearts=5;
            revives--;
        }
    }
    void incrementRevives() {
        revives++;
    }
    void resetTimer(RenderWindow& window) {
        PowerUpTimer.restart();
        revive.setPosition(Vector2f(rand() % (window.getSize().x - 100), 0));
    }

    int getRevives() {
        return revives;
    }
};


class SpeedBoost {
    Picture speedboost;
    Clock PowerUpTimer;
   
    public:
    SpeedBoost(RenderWindow& window, string speedboostText) : speedboost(speedboostText) {
        speedboost.setScale(Vector2f(50, 50));
        speedboost.setPosition(Vector2f(rand() % (window.getSize().x - 100), 0));
    }
    void drawTo(RenderWindow& window) {
        speedboost.drawTo(window);
    }

    void move(double movement) {
        speedboost.move(0, movement * 2);
    }

    FloatRect getGlobalBounds() {
        return speedboost.getGlobalBounds();
    }
    
    int timer = rand() % 40;
    
    void script(RenderWindow& window, Spaceship& spaceship,double &movement) {
        if (PowerUpTimer.getElapsedTime().asSeconds() > timer) {
            speedboost.move(0, 2);
            speedboost.drawTo(window);
            if (spaceship.getGlobalBounds().intersects(speedboost.getGlobalBounds())) {
                movement *= 5.0;
                PowerUpTimer.restart();
                speedboost.setPosition(Vector2f(rand() % (window.getSize().x - 100), 0));
            }
            else if (speedboost.getPosition().y > window.getSize().y) {
                PowerUpTimer.restart();
                speedboost.setPosition(Vector2f(rand() % (window.getSize().x - 100), 0));
            }
        }
    }

    void Timeout(double &movement) {
            if (PowerUpTimer.getElapsedTime().asSeconds() > 10) {
                movement = 15.0;
            }
    }
};
  
class HealthRegen {
    Picture healthregen;
    Clock PowerUpTimer;
    public:
    HealthRegen(RenderWindow& window, string healthregenText) : healthregen(healthregenText) {
        healthregen.setScale(Vector2f(50, 50));
        healthregen.setPosition(Vector2f(rand() % (window.getSize().x - 100), 0));
    }
    void drawTo(RenderWindow& window) {
        healthregen.drawTo(window);
    }

    void move(double movement) {
        healthregen.move(0, movement * 2);
    }

    FloatRect getGlobalBounds() {
        return healthregen.getGlobalBounds();
    }
    int timer = rand() % 60;
    void script(RenderWindow& window, Spaceship& spaceship, int &hearts) {
        if (PowerUpTimer.getElapsedTime().asSeconds() > timer) {
            healthregen.move(0, 2);
            healthregen.drawTo(window);
            if (spaceship.getGlobalBounds().intersects(healthregen.getGlobalBounds())) {
                hearts += 2;
                PowerUpTimer.restart();
                healthregen.setPosition(Vector2f(rand() % (window.getSize().x - 100), 0));
            }
            else if (healthregen.getPosition().y > window.getSize().y) {
                PowerUpTimer.restart();
                healthregen.setPosition(Vector2f(rand() % (window.getSize().x - 100), 0));
            }
        }
    }
};

bool GameOver(RenderWindow& window,int score) {
    Button GoTomain("MAIN MENU", Vector2f(300, 80), 24, Color::Transparent, Color::White);
    Button PlayAgain("PLAY AGAIN", Vector2f(300, 80), 24, Color::Transparent, Color::White);
    Text GameOver;

     Texture GO;
    if (!GO.loadFromFile("GameO.png")) {
        window.close();
    }
    Sprite GameO;
    GameO.setTexture(GO);

    Font font;
    Font font_f;

    if (!font_f.loadFromFile("Thorletto.otf")) {
        window.close();
    }

    if (!font.loadFromFile("BuckBoard.ttf")) {
        window.close();
    }
    GameOver = Text("Game Over", font_f, 55);
    Text Score = Text("Score: "+to_string(score), font, 20);
    GameOver.setPosition(Vector2f(500,325));
    GoTomain.setFont(font);
    PlayAgain.setFont(font);
    Score.setPosition((window.getSize().x / 2.0) - 60, (window.getSize().y / 2.0));
    GoTomain.setPosition(Vector2f((window.getSize().x / 2.0) - PlayAgain.getSize().x - 5, window.getSize().y / 2 + 100));
    PlayAgain.setPosition(Vector2f((window.getSize().x / 2.0) + 5, window.getSize().y / 2 + 100));
    while (window.isOpen()) {
        window.clear();
        
        window.draw(GameO);
        GoTomain.drawTo(window);
        PlayAgain.drawTo((window));
       
        Event event;
        while(window.pollEvent(event))
        {
            if(event.type==Event::Closed)
            {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed) {
                if (GoTomain.buttonClicked(window)) {
                    return true;
                }
                if (PlayAgain.buttonClicked(window)) {
                    return false;
                }
            }
        }
        window.draw(GameOver);
        window.draw(Score);
        
        window.display();
    }
   return true;
}
