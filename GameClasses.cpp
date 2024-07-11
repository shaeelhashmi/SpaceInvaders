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
    Button(string t, Vector2f size, int charSize, Color bgColor, Color textColor, float padding = 1.0f)
        : padding(padding), charSize(charSize) {
        text.setString(t);
        text.setFillColor(textColor);
        text.setCharacterSize(charSize);
        button.setSize(size);
        button.setFillColor(bgColor);
        centerText();
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

    void setText(string t) {
        text.setString(t);
        adjustTextSize();
        centerText();
    }

    void setPosition(Vector2f pos) {
        button.setPosition(pos);
        centerText();
    }

    void setPadding(float newPadding) {
        padding = newPadding;
        adjustTextSize();
        centerText();
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
    float padding;
    int charSize;

    void centerText() {
        FloatRect textBounds = text.getLocalBounds();
        FloatRect buttonBounds = button.getLocalBounds();
        float xPos = button.getPosition().x + (buttonBounds.width - textBounds.width) / 2 - textBounds.left;
        float yPos = button.getPosition().y + (buttonBounds.height - textBounds.height) / 2 - textBounds.top;
        text.setPosition(xPos, yPos);
    }

    void adjustTextSize() {
        FloatRect buttonBounds = button.getLocalBounds();
        float maxWidth = buttonBounds.width - 2 * padding;
        float maxHeight = buttonBounds.height - 2 * padding;

        text.setCharacterSize(charSize);
        FloatRect textBounds = text.getLocalBounds();

        if (textBounds.width > maxWidth || textBounds.height > maxHeight) {
            float widthScale = maxWidth / textBounds.width;
            float heightScale = maxHeight / textBounds.height;

            float scale = std::min(widthScale, heightScale);

            text.setCharacterSize(static_cast<int>(charSize * scale));
        }
    }
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
class Powerups{
protected:
    Picture powerup;
    Clock PowerUpTimer;
public:
Powerups(string text,RenderWindow &window):powerup(text){
    powerup.setScale(Vector2f(50,50));
    powerup.setPosition(Vector2f(rand() % (window.getSize().x - 100), 0));
}
void move(double movement){
    powerup.move(0, movement * 2);
}
void drawTo(RenderWindow& window){
    powerup.drawTo(window);
}
void ResetTimer(RenderWindow& window){
    PowerUpTimer.restart();
   powerup.setPosition(Vector2f(rand() % (window.getSize().x - 100), 0));
}
FloatRect getGlobalBounds(){
    return powerup.getGlobalBounds();
}
};
class Revive:public Powerups    {

    int revives;
    int timer;
public:
    Revive(RenderWindow& window, string reviveText) : Powerups(reviveText,window) {
        timer = rand() % 120;
        revives=0;
    }
    void increaseRevives() {
        revives++;
    }
    
    void script(RenderWindow& window,Spaceship& spaceship)  {
        if(PowerUpTimer.getElapsedTime().asSeconds()> timer){    
            powerup.move(0, 2);
            powerup.drawTo(window);
            if(spaceship.getGlobalBounds().intersects(powerup.getGlobalBounds())){
                increaseRevives();
                powerup.setPosition(Vector2f(rand() % (window.getSize().x - 100), 0));
                PowerUpTimer.restart();
                timer=rand()%120;        
            }
            else if(powerup.getPosition().y>window.getSize().y){
                PowerUpTimer.restart();
                powerup.setPosition(Vector2f(rand() % (window.getSize().x - 100), 0));
                timer=rand()%120;
                
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
    int getRevives() {
        return revives;
    }
};


class SpeedBoost:public Powerups {
     int timer ;
    public:
    SpeedBoost(RenderWindow& window, string speedboostText) : Powerups(speedboostText,window) {
        timer=rand()%40;
    }   
    void script(RenderWindow& window, Spaceship& spaceship,double &movement) {
        if (PowerUpTimer.getElapsedTime().asSeconds() > timer) {
            powerup.move(0, 2);
            powerup.drawTo(window);
            if (spaceship.getGlobalBounds().intersects(powerup.getGlobalBounds())) {
                movement *= 3.0;
                PowerUpTimer.restart();
                powerup.setPosition(Vector2f(rand() % (window.getSize().x - 100), 0));
                timer=rand()%40;
            }
            else if (powerup.getPosition().y > window.getSize().y) {
                PowerUpTimer.restart();
                powerup.setPosition(Vector2f(rand() % (window.getSize().x - 100), 0));
                timer=rand()%40;
            }
        }
    }
    void Timeout(double &movement) {
            if (PowerUpTimer.getElapsedTime().asSeconds() > 10) {
                movement = 15.0;
            }
    }
};
  
class HealthRegen:public Powerups{
int timer ;
    public:
    HealthRegen(RenderWindow& window, string healthregenText) : Powerups(healthregenText,window) {
        timer=rand()%60;
    }
 
    void script(RenderWindow& window, Spaceship& spaceship, int &hearts) {
        if (PowerUpTimer.getElapsedTime().asSeconds() > timer) {
            powerup.move(0, 2);
            powerup.drawTo(window);
            if (spaceship.getGlobalBounds().intersects(powerup.getGlobalBounds())) {
                if(hearts<3)
                {
                hearts += 2;
                }
                else
                {
                    hearts=5;
                }
                PowerUpTimer.restart();
                powerup.setPosition(Vector2f(rand() % (window.getSize().x - 100), 0));
                timer=rand()%60;
            }
            else if (powerup.getPosition().y > window.getSize().y) {
                PowerUpTimer.restart();
                powerup.setPosition(Vector2f(rand() % (window.getSize().x - 100), 0));
                timer=rand()%60;
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
