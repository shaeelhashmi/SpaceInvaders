#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>

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
    bool checkleft(double move) {
        return spaceship.getPosition().x - move > 0;
    }
    bool checkright(RenderWindow& window, double move) {
        return spaceship.getPosition().x + move < window.getSize().x - 100;
    }
bool checkUp(double move,RenderWindow& window) {
        return spaceship.getPosition().y - move > window.getSize().y/2;
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
    Asteroid(RenderWindow& window, string Filepath, double Corners) : asteroid(Filepath) {
        size = (rand() % 2) + 1;

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
    int getSizeValue() {
        return size;
    }
};

class Bullets {
    Picture bullet;

public:
    Bullets(RenderWindow& window,string Texture) : bullet(Texture) {
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
    int hits;
public:
    Boss(RenderWindow& window, string bossText) : boss(bossText) {
        boss.setScale(Vector2f(window.getSize().x * 0.082, window.getSize().y * 0.134));
        boss.setPosition(Vector2f((window.getSize().x / 2) - 100, 100));
        boss.rotate(180);
        hits=0;
    }
    void script(RenderWindow& window) {
        srand(time(nullptr));
        if (changeMovement.getElapsedTime().asSeconds() > 8) {
            float positionX = rand() % (window.getSize().x-200);
            float positionY = (rand() % window.getSize().y / 4)+200;
            boss.setPosition(Vector2f(positionX, positionY));
            changeMovement.restart();
        }
        drawTo(window);
        if(hits==10)
        {
            cout<<"Boss is dead";
        }
    }
    void incrementHits(){
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
};

//This class is for minions that will spawn in each level
class AirStrikeMinion{
    Picture minion;
    public:
    AirStrikeMinion(RenderWindow& window,string minionText):minion(minionText){
        minion.setScale(Vector2f(window.getSize().x * 0.082, window.getSize().y * 0.134));
        minion.setPosition(Vector2f(window.getSize().x-100, 100));
    }
    void script(RenderWindow& window){
        minion.move(-4,0);
        drawTo(window);
    }
    void drawTo(RenderWindow& window){
        minion.drawTo(window);
    }
    Vector2f getPosition(){
        return minion.getPosition();
    }
    void setPosition(Vector2f position){
        minion.setPosition(position);
    }
};
int main() {
    RenderWindow window(VideoMode::getDesktopMode(), "Space invader", Style::Close | Style::Fullscreen);
    window.setFramerateLimit(60);
    vector<Asteroid> asteroids;
    //This array will store the exploded array
    vector<Asteroid> explodedAsteroids;
    //This vector will store the time for each exploded asteroid
    vector<Clock> explodedAsteroidsTime;
    Clock asteroidClock;
      //The things below are related to the spaceship and the boss
    Spaceship spaceship(window);
    Bullets bullet(window,"bullets.png");
    Bullets bossBullet(window,"Bombs.png");
    Picture hearts[5] = { Picture("hearts.png"), Picture("hearts.png"), Picture("hearts.png"), Picture("hearts.png"), Picture("hearts.png") };
    int levels = 1;
    Clock clock;
    Boss b1(window, "Boss.png");
    vector<Bullets> bullets;
    vector<Bullets> bossBullets;
//This Clock shows the bullets shot by the boss
Clock bossShoot;
//These variable are for the minions
AirStrikeMinion minion(window,"MinionShip.png");
vector<Bullets> minionBullets;
Bullets minionBullet(window,"AirBomb.png");
Clock startStrike;
bool startStrikeBool=false;
Clock minionShoot;
//These variables set small details of the game
    int heart = 5;
    int multiplier = 1;
    bool a = true;
    double movement = window.getSize().x * window.getSize().y * 0.0000039 * 2;
    int score = 0;
    int highScore = 0;
    ifstream HighScoreInput("highscore.txt");
    if (HighScoreInput.is_open()) {
        HighScoreInput >> highScore;
        HighScoreInput.close();
    }

    Font font;
    if (!font.loadFromFile("AGENCYR.ttf")) {
        cout << "Error loading font" << endl;
    }
    Text scoretxt, highscoretxt,levelsTxt;
    scoretxt.setFont(font);
    scoretxt.setCharacterSize(24);
    scoretxt.setFillColor(Color::White);
    scoretxt.setPosition(window.getSize().x - 150, 10);

    highscoretxt.setFont(font);
    highscoretxt.setCharacterSize(24);
    highscoretxt.setFillColor(Color::White);
    highscoretxt.setPosition(window.getSize().x - 150, 40);

    levelsTxt.setFont(font);
    levelsTxt.setCharacterSize(24);
    levelsTxt.setFillColor(Color::White);
    levelsTxt.setPosition(window.getSize().x - 150, 80);

    for (int i = 0; i < 5; i++) {
        hearts[i].setScale(Vector2f(40, 40));
    }
    for (int i = 0; i < 5; i++) {
        static int pos = 10;
        hearts[i].setPosition(Vector2f(pos, 20));
        pos += 50;
    }
    
    while (window.isOpen()) {

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        if (event.key.code == Keyboard::Space && (clock.getElapsedTime().asSeconds() > 1 || a)) {
            bullet.SetPosition((spaceship.getPosition().x)+50 , spaceship.getPosition().y - 10);
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
        if ((Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) && spaceship.checkUp(movement,window)) {
            spaceship.move(0, -1 * movement);
        }
        if ((Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) && spaceship.checkdown(window, movement)) {
            spaceship.move(0, movement);
        }
        window.clear();
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
                    int asteroidSize = asteroids[i].getSizeValue();
                    if (asteroidSize == 1) {
                        score += 5 * multiplier;
                    }
                    else if (asteroidSize == 2) {
                        score += 10 * multiplier;
                    }
                    asteroids.erase(asteroids.begin() + i);
                    bullets.erase(bullets.begin() + j);
                }
            }
        }
          if (asteroidClock.getElapsedTime().asSeconds() > 3.0f) {
            Asteroid as(window, "Asteroid.png", spaceship.getSize().x);
            asteroids.push_back(as);
            asteroidClock.restart();
        }
        for (int i = 0; i < explodedAsteroids.size(); i++) {
            if (explodedAsteroidsTime[i].getElapsedTime().asSeconds() > 1) {
                explodedAsteroids.erase(explodedAsteroids.begin() + i);
                explodedAsteroidsTime.erase(explodedAsteroidsTime.begin() + i);
            }
            else {
                explodedAsteroids[i].SetTexture("AsteroidDestructions.png");
                explodedAsteroids[i].drawTo(window);
            }
        }
        //These functions will spawn the asteroids in the window
        for (int i = 0; i < bullets.size(); i++) {
            if (bullets[i].getPosition().y < 0) {
                bullets.erase(bullets.begin() + i);
            }
            else {
                bullets[i].move(0, window.getSize().y * -0.02);
                bullets[i].drawTo(window);
            }
        }
   
        b1.script(window);
        //This condition will check if the spaceship bullets has collided with the boss
        for(int i=0;i<bullets.size();i++){
            if(bullets[i].getGlobalBounds().intersects(b1.getGlobalBounds())){
                b1.incrementHits();
                bullets.erase(bullets.begin()+i);
                
            }
        }
        //This condition will check if the boss bullets has collided with the spaceship
        for(int i=0;i<bossBullets.size();i++){
            if(bossBullets[i].getGlobalBounds().intersects(spaceship.getGlobalBounds())){
                heart-=2;
                bossBullets.erase(bossBullets.begin()+i);
            }
        }
        //This condition will check if the boss bullets are out of bounds
        for(int i=0;i<bossBullets.size();i++){
            if(bossBullets[i].getPosition().y>window.getSize().y){
                bossBullets.erase(bossBullets.begin()+i);
            }
        }
        if (bossShoot.getElapsedTime().asSeconds() > 4.5) {
            bossBullet.SetPosition(b1.getPosition().x-50, b1.getPosition().y);
            bossBullets.push_back(bossBullet);
            bossShoot.restart();
        }
        for(int i=0;i<bossBullets.size();i++){
            if(bossBullets[i].getPosition().y>0){
                bossBullets[i].move(0,window.getSize().y*0.002);
                bossBullets[i].drawTo(window);
            }
            else{
                bossBullets.erase(bossBullets.begin()+i);
            }
        }
        spaceship.drawTo(window);
        for (int i = 0; i < heart; i++) {
            hearts[i].drawTo(window);
        }
        if (heart <= 0) {
            cout << "GAME OVER";
           
            if (score > highScore) {
                highScore = score;
                ofstream HighScore("highscore.txt", ios::app);
                if (HighScore.is_open()) {
                    HighScore << highScore << "\n";
                    HighScore.close();
                }

            }
            window.close();
        }
        //This condition is for the air strike of the minions
        if(startStrike.getElapsedTime().asSeconds()>5){
            startStrikeBool=true;
        }
        if(startStrikeBool){
            minion.script(window);
            if(minion.getPosition().x<0){
                minion.setPosition(Vector2f(window.getSize().x-100,100));
                startStrikeBool=false;
                startStrike.restart();
            }         
        }
        //So that the minions can shoot the bullets after two seconds
        if(minionShoot.getElapsedTime().asSeconds()>2&&startStrikeBool){
            minionBullet.SetPosition(minion.getPosition().x+50,minion.getPosition().y+100);
            minionBullets.push_back(minionBullet);
            minionShoot.restart();
        }
         for(int i=0;i<minionBullets.size();i++){
                if(minionBullets[i].getGlobalBounds().intersects(spaceship.getGlobalBounds())){
                    heart--;
                    minionBullets.erase(minionBullets.begin()+i);
                }
                else if(minionBullets[i].getPosition().y<window.getSize().y){
                    minionBullets[i].move(0,window.getSize().y*0.002);
                    minionBullets[i].drawTo(window);
                }
                else{
                    minionBullets.erase(minionBullets.begin()+i);
                }
            }
      
        scoretxt.setString("Score: " + to_string(score));
        highscoretxt.setString("High Score: " + to_string(highScore));
        levelsTxt.setString("Level: " + to_string(levels));
        window.draw(scoretxt);
        window.draw(highscoretxt);
        window.display();    
    }
    return 0;
}