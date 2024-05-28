#include "GameClasses.cpp"
//This is the first boss that will spawn after the first 4 levels
void setVal(double &bulletSpeed,double &teleportationTimer,string Setting[],RenderWindow &window){
    if(Setting[1] == "easy"){
         teleportationTimer = teleportationTimer+2.0;
        bulletSpeed =window.getSize().y*(bulletSpeed-0.002);
    }
    else if(Setting[1] == "medium"){
    teleportationTimer = teleportationTimer+2;
       bulletSpeed=window.getSize().y*(bulletSpeed-0.002);
    }
    else if(Setting[1] == "hard"){
        teleportationTimer = teleportationTimer;
        bulletSpeed =window.getSize().y*(bulletSpeed-0.002);
        cout<<bulletSpeed;
    }
}
void level1Boss(RenderWindow& window, int& levels, Spaceship& spaceship, vector<Bullets>& bullets, int& heart, int& score, int highScore, Picture hearts[], Text& scoretxt, Text& highscoretxt, Text& levelsTxt, Bullets& bullet, Clock& clock, double movement,string Setting[],Sound &HealthLoss,Sound &GameSound,Revive &revive) {
    spaceship.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y - 100));
    window.clear();
    double teleportationTimer = 10;
    double bulletSpeed = 0.007;
    setVal(bulletSpeed,teleportationTimer,Setting,window);
    //Creating bullet sound
   SoundBuffer bulletBuffer;
    SoundBuffer BossbulletsBuffer;
      if(Setting[0]=="0")
    {
        bulletBuffer.loadFromFile("audios/NoSound.wav");
        BossbulletsBuffer.loadFromFile("audios/NoSound.wav");
    }
    else
    {
    if (!bulletBuffer.loadFromFile("audios/BulletShoot.wav")) {
        cout << "Error loading bullet sound" << endl;
    }

     BossbulletsBuffer.loadFromFile("audios/BossBullet.wav");
    }
    Sound bulletSound(bulletBuffer); 
    Sound BossBulletsSound(BossbulletsBuffer);

    bool a = false;
    Boss b1(window, "Boss1.png");
    Clock bossShoot;
    vector<Bullets> bossBullets;
    Bullets bossBullet(window, "Boss1Bullet.png");
    Font font;
    if (!font.loadFromFile("AGENCYR.ttf")) {
        cout << "Error loading font" << endl;
    }
    Text Welcome = Text("Welcome to the first boss fight", font, 50);
    Welcome.setPosition(window.getSize().x / 2 - 200, window.getSize().y / 2);
    Clock TextTimer;
    while (TextTimer.getElapsedTime().asSeconds() < 5) {

        window.clear();
        window.draw(Welcome);
        window.display();
    }
    GameSound.play();
    Clock gameSound;
    while (window.isOpen()) {
        window.clear();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        if (event.key.code == Keyboard::Space && (clock.getElapsedTime().asSeconds() > 0.5)) {
            bulletSound.play();
            bullet.SetPosition((spaceship.getPosition().x) + 50, spaceship.getPosition().y - 10);
            bullets.push_back(bullet);
            bullet.drawTo(window);
            window.display();
            clock.restart();
            event.key.code = Keyboard::Unknown;
        }
        if ((Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) && spaceship.checkleft(movement)) {
            spaceship.move(-1 * movement, 0);
        }
        if ((Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) && spaceship.checkright(window, movement)) {
            spaceship.move(movement, 0);
        }
        if ((Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) && spaceship.checkUp(movement, window)) {
            spaceship.move(0, -1 * movement);

        }
        if ((Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) && spaceship.checkdown(window, movement)) {
            spaceship.move(0, movement);

        }

        //This condition will check if the multiplier has ended
        window.clear();
        for (int i = 0; i < bullets.size(); i++) {
            if (bullets[i].getPosition().y < 0) {
                bullets.erase(bullets.begin() + i);
            }
            else {
                bullets[i].move(0, window.getSize().y * -0.02);
                bullets[i].drawTo(window);
            }
        }

        
        //This condition will check if the spaceship bullets has collided with the boss
        for (int i = 0;i < bullets.size();i++) {
            if (bullets[i].getGlobalBounds().intersects(b1.getGlobalBounds())) {
                b1.incrementHits();
                bullets.erase(bullets.begin() + i);
                if (b1.getHits() == 5) {
                    levels++;
                    score += 500;
                    a = true;
                }
            }
        }
        //This condition will check if the boss bullets has collided with the spaceship
        for (int i = 0;i < bossBullets.size();i++) {
            if (bossBullets[i].getGlobalBounds().intersects(spaceship.getGlobalBounds())) {
                HealthLoss.play();
                heart--;
                bossBullets.erase(bossBullets.begin() + i);
            }
        }
        //This condition will check if the boss bullets are out of bounds
        for (int i = 0;i < bossBullets.size();i++) {
            if (bossBullets[i].getPosition().y > window.getSize().y) {
                bossBullets.erase(bossBullets.begin() + i);
            }
        }
        if (bossShoot.getElapsedTime().asSeconds() > 3) {
            BossBulletsSound.play();
            bossBullet.SetPosition(b1.getPosition().x + 50, b1.getPosition().y + 50);
            bossBullets.push_back(bossBullet);
            bossShoot.restart();
        }
        if (a) {
            break;
        }
        for (int i = 0;i < bossBullets.size();i++) {
            if (bossBullets[i].getPosition().y > 0) {
                bossBullets[i].move(0, bulletSpeed);
                bossBullets[i].drawTo(window);
            }
            else {
                bossBullets.erase(bossBullets.begin() + i);
            }
        }
        b1.script(window, spaceship, teleportationTimer);
        spaceship.drawTo(window);
        for (int i = 0; i < heart; i++) {
            hearts[i].drawTo(window);
        }
        if (heart <= 0) {
            revive.SetHealth(heart);
            if(heart<=0)
            {
                return;
            }
            else{
                  continue;
            }
        }
       if(gameSound.getElapsedTime().asSeconds()>GameSound.getBuffer()->getDuration().asSeconds())
        {
            GameSound.play();
            gameSound.restart();
        }
        scoretxt.setString("Score: " + to_string(score));
        highscoretxt.setString("High Score: " + to_string(highScore));
        levelsTxt.setString("Level: " + to_string(levels));
        window.draw(scoretxt);
        window.draw(highscoretxt);
        window.draw(levelsTxt);
        window.display();
    }
    TextTimer.restart();
    Text Outro = Text("Congratulations you have passed the first boss fight", font, 50);
    Outro.setPosition(Welcome.getPosition().x - 300, Welcome.getPosition().y / 2);
    while (TextTimer.getElapsedTime().asSeconds() < 5) {
        window.clear();
        window.draw(Outro);
        window.display();
    }
    spaceship.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y - 100));
}
void level2Boss(RenderWindow& window, int& levels, Spaceship& spaceship, vector<Bullets>& bullets, int& heart, int& score, int highScore, Picture hearts[], Text& scoretxt, Text& highscoretxt, Text& levelsTxt, Bullets& bullet, Clock& clock, double movement,string Setting[],Sound &HealthLoss,Sound &GameSound,Revive &revive) {
    spaceship.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y - 100));
   double teleportationTimer = 8;
    double bulletSpeed = 0.009;
    setVal(bulletSpeed,teleportationTimer,Setting,window);
    SoundBuffer bulletBuffer;
    SoundBuffer BossbulletsBuffer;
      if(Setting[0]=="0")
    {
        bulletBuffer.loadFromFile("audios/NoSound.wav");
        BossbulletsBuffer.loadFromFile("audios/NoSound.wav");
    }
    else
    {
    if (!bulletBuffer.loadFromFile("audios/BulletShoot.wav")) {
        cout << "Error loading bullet sound" << endl;
    }

     BossbulletsBuffer.loadFromFile("audios/BossBullet.wav");
    }
    Sound bulletSound(bulletBuffer); 
    Sound BossBulletsSound(BossbulletsBuffer);

    bulletSound.setBuffer(bulletBuffer);
    bool a = false;
    Boss b1(window, "Boss2.png");
    Clock bossShoot;
    vector<Bullets> bossBullets;
    Bullets bossBullet(window, "Boss2Bullet.png");
    Font font;
    if (!font.loadFromFile("AGENCYR.ttf")) {
        cout << "Error loading font" << endl;
    }
    Text Welcome = Text("Welcome to the second boss fight", font, 50);
    Welcome.setPosition(window.getSize().x / 2 - 200, window.getSize().y / 2);
    Clock TextTimer;
    while (TextTimer.getElapsedTime().asSeconds() < 5) {
        window.clear();
        window.draw(Welcome);
        window.display();
    }
    window.clear();
    Clock gameSound;
    GameSound.play();
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        b1.Follow(spaceship, movement);
        if (event.key.code == Keyboard::Space && (clock.getElapsedTime().asSeconds() > 0.5)) {
            bulletSound.play();
            bullet.SetPosition((spaceship.getPosition().x) + 50, spaceship.getPosition().y - 10);
            bullets.push_back(bullet);
            bullet.drawTo(window);
            window.display();
            clock.restart();

            event.key.code = Keyboard::Unknown;
        }
        if ((Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) && spaceship.checkleft(movement)) {
            spaceship.move(-1 * movement, 0);
        }
        if ((Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) && spaceship.checkright(window, movement)) {
            spaceship.move(movement, 0);
        }
        if ((Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) && spaceship.checkUp(movement, window)) {
            spaceship.move(0, -1 * movement);

        }
        if ((Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) && spaceship.checkdown(window, movement)) {
            spaceship.move(0, movement);

        }

        //This condition will check if the multiplier has ended
        window.clear();
        b1.script(window, spaceship, teleportationTimer);
        for (int i = 0; i < bullets.size(); i++) {
            if (bullets[i].getPosition().y < 0) {
                bullets.erase(bullets.begin() + i);
            }
            else {
                bullets[i].move(0, window.getSize().y * -0.02);
                bullets[i].drawTo(window);
            }
        }

       
        //This condition will check if the spaceship bullets has collided with the boss
        for (int i = 0;i < bullets.size();i++) {
            if (bullets[i].getGlobalBounds().intersects(b1.getGlobalBounds())) {
                b1.incrementHits();
                bullets.erase(bullets.begin() + i);
                if (b1.getHits() == 5) {
                    levels++;
                    score += 500;
                    a = true;
                }
            }
        }
        //This condition will check if the boss bullets has collided with the spaceship
        for (int i = 0;i < bossBullets.size();i++) {
            if (bossBullets[i].getGlobalBounds().intersects(spaceship.getGlobalBounds())) {
                HealthLoss.play();
                heart--;
                bossBullets.erase(bossBullets.begin() + i);

            }
        }
        //This condition will check if the boss bullets are out of bounds
        for (int i = 0;i < bossBullets.size();i++) {
            if (bossBullets[i].getPosition().y > window.getSize().y) {
                bossBullets.erase(bossBullets.begin() + i);
            }
        }
        if (bossShoot.getElapsedTime().asSeconds() > 2) {
            BossBulletsSound.play();
            bossBullet.SetPosition(b1.getPosition().x + 50, b1.getPosition().y + 50);
            bossBullets.push_back(bossBullet);
            bossShoot.restart();
        }
        if (a) {
            break;
        }
        for (int i = 0;i < bossBullets.size();i++) {
            if (bossBullets[i].getPosition().y > 0) {
                bossBullets[i].move(0, bulletSpeed);
                bossBullets[i].drawTo(window);
            }
            else {
                bossBullets.erase(bossBullets.begin() + i);
            }
        }
        spaceship.drawTo(window);
        for (int i = 0; i < heart; i++) {
            hearts[i].drawTo(window);
        }
        if (heart <= 0) {
  revive.SetHealth(heart);
            if(heart<=0)
            {
                return;
            }
            else{
                  continue;
            }
        }
          if(gameSound.getElapsedTime().asSeconds()>GameSound.getBuffer()->getDuration().asSeconds())
        {
            GameSound.play();
            gameSound.restart();
        }
        scoretxt.setString("Score: " + to_string(score));
        highscoretxt.setString("High Score: " + to_string(highScore));
        levelsTxt.setString("Level: " + to_string(levels));
        window.draw(scoretxt);
        window.draw(highscoretxt);
        window.draw(levelsTxt);
        window.display();
    }
    window.clear();
    TextTimer.restart();
    Text Outro = Text("Congratulations you have passed the Second boss fight", font, 50);
    Outro.setPosition(Welcome.getPosition().x - 300, Welcome.getPosition().y);

    while (TextTimer.getElapsedTime().asSeconds() < 5) {
        window.clear();
        window.draw(Outro);
        window.display();
    }
    spaceship.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y - 100));
}
void level3Boss(RenderWindow& window, int& levels, Spaceship& spaceship, vector<Bullets>& bullets, int& heart, int& score, int highScore, Picture hearts[], Text& scoretxt, Text& highscoretxt, Text& levelsTxt, Bullets& bullet, Clock& clock, double movement, Asteroid& as, vector<Asteroid>& asteroids, vector<Asteroid>& explodedAsteroids, vector<Clock>& explodedAsteroidsTime, Clock& asteroidClock, int& multiplier, Clock& endMultiplier, Text& Multiplier, string settings[],Sound &HealthLoss,Sound &GameSound,Revive &revive) {
    window.clear();
   double teleportationTimer = 8;
    double bulletSpeed = 0.01;
    setVal(bulletSpeed,teleportationTimer,settings,window);
    int temp = multiplier;
    SoundBuffer bulletBuffer;
    SoundBuffer asteroidSoundbuffer;
    SoundBuffer BossbulletsBuffer;
   if(settings[0]=="0")
    {
        bulletBuffer.loadFromFile("audios/NoSound.wav");
        asteroidSoundbuffer.loadFromFile("audios/NoSound.wav");
        BossbulletsBuffer.loadFromFile("audios/NoSound.wav");
    }
    else
    {
    if (!bulletBuffer.loadFromFile("audios/BulletShoot.wav")) {
        cout << "Error loading bullet sound" << endl;
    }
    asteroidSoundbuffer.loadFromFile("audios/AsteroidExplosion.wav");
     BossbulletsBuffer.loadFromFile("audios/BossBullet.wav");
    }
    Sound bulletSound(bulletBuffer); 
    Sound BossBulletsSound(BossbulletsBuffer);
    Sound asteroidSound(asteroidSoundbuffer);

    bulletSound.setBuffer(bulletBuffer);
    spaceship.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y - 100));
    bool a = false;
    Boss b1(window, "Boss3.png");
    Clock bossShoot;
    vector<Bullets> bossBullets;
    Bullets bossBullet(window, "Boss3Bullet.png");
    Font font;
    if (!font.loadFromFile("AGENCYR.ttf")) {
        cout << "Error loading font" << endl;
    }
    Text Welcome = Text("Welcome to the Third boss fight", font, 50);
    Welcome.setPosition(window.getSize().x / 2 - 200, window.getSize().y / 2);
    Clock TextTimer;
    while (TextTimer.getElapsedTime().asSeconds() < 5) {
        window.clear();
        window.draw(Welcome);
        window.display();
    }
    window.clear();
    multiplier = temp;
    endMultiplier.restart();
    Clock gameSound;
    GameSound.play();
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        b1.Follow(spaceship, movement);
        if (event.key.code == Keyboard::Space && (clock.getElapsedTime().asSeconds() > 0.5)) {
            bulletSound.play();
            bullet.SetPosition((spaceship.getPosition().x) + 50, spaceship.getPosition().y - 10);
            bullets.push_back(bullet);
            bullet.drawTo(window);
            window.display();
            clock.restart();

            event.key.code = Keyboard::Unknown;
        }
        if ((Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) && spaceship.checkleft(movement)) {
            spaceship.move(-1 * movement, 0);
        }
        if ((Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) && spaceship.checkright(window, movement)) {
            spaceship.move(movement, 0);
        }
        if ((Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) && spaceship.checkUp(movement, window)) {
            spaceship.move(0, -1 * movement);

        }
        if ((Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) && spaceship.checkdown(window, movement)) {
            spaceship.move(0, movement);

        }

        //This condition will check if the multiplier has ended
        window.clear();
        b1.script(window, spaceship, 8);
        for (int i = 0; i < bullets.size(); i++) {
            if (bullets[i].getPosition().y < 0) {
                bullets.erase(bullets.begin() + i);
            }
            else {
                bullets[i].move(0, window.getSize().y * -0.02);
                bullets[i].drawTo(window);
            }
        }
        for (int i = 0; i < asteroids.size(); i++) {
            asteroids[i].SetTexture("Asteroid.png");
            asteroids[i].move(window, levels,settings[1]);
            asteroids[i].drawTo(window);
            if (spaceship.getGlobalBounds().intersects(asteroids[i].getGlobalBounds())) {
                heart--;
                asteroids.erase(asteroids.begin() + i);
            }
            if (asteroids[i].getPosition().y > window.getSize().y) {
                asteroids.erase(asteroids.begin() + i);
            }
            for (int j = 0; j < bullets.size(); j++) {
                if (asteroids[i].getGlobalBounds().intersects(bullets[j].getGlobalBounds())) {
                    asteroidSound.play();
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
                    multiplier++;
                    endMultiplier.restart();
                }
            }
        }
        if (endMultiplier.getElapsedTime().asSeconds() > 3) {
            multiplier = 1;
        }

        if (asteroidClock.getElapsedTime().asSeconds() > 3.0f) {
            as.setRandomPosition(window);
            as.setRandomSize();
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

        
        //This condition will check if the spaceship bullets has collided with the boss
        for (int i = 0;i < bullets.size();i++) {
            if (bullets[i].getGlobalBounds().intersects(b1.getGlobalBounds())) {
                b1.incrementHits();
                bullets.erase(bullets.begin() + i);
                if (b1.getHits() == 8) {
                    levels++;
                    score += 500;
                    a = true;
                }
            }
        }
        //This condition will check if the boss bullets has collided with the spaceship
        for (int i = 0;i < bossBullets.size();i++) {
            if (bossBullets[i].getGlobalBounds().intersects(spaceship.getGlobalBounds())) {
                HealthLoss.play();
                heart -= 2;
                bossBullets.erase(bossBullets.begin() + i);
            }
        }
        //This condition will check if the boss bullets are out of bounds
        for (int i = 0;i < bossBullets.size();i++) {
            if (bossBullets[i].getPosition().y > window.getSize().y) {
                bossBullets.erase(bossBullets.begin() + i);
            }
        }
        if (bossShoot.getElapsedTime().asSeconds() > 2) {
            BossBulletsSound.play();
            bossBullet.SetPosition(b1.getPosition().x + 50, b1.getPosition().y + 50);
            bossBullets.push_back(bossBullet);
            bossShoot.restart();
        }
        if (a) {
            break;
        }
        for (int i = 0;i < bossBullets.size();i++) {
            if (bossBullets[i].getPosition().y > 0) {
                bossBullets[i].move(0,bulletSpeed);
                bossBullets[i].drawTo(window);
            }
            else {
                bossBullets.erase(bossBullets.begin() + i);
            }
        }
        spaceship.drawTo(window);
        for (int i = 0; i < heart; i++) {
            hearts[i].drawTo(window);
        }
        if (heart <= 0) {
            revive.SetHealth(heart);
            if(heart<=0)
            {
                return;
            }
            else{
                  continue;
            }
        }
        if(gameSound.getElapsedTime().asSeconds()>GameSound.getBuffer()->getDuration().asSeconds())
        {
            GameSound.play();
            gameSound.restart();
        }
        scoretxt.setString("Score: " + to_string(score));
        highscoretxt.setString("High Score: " + to_string(highScore));
        levelsTxt.setString("Level: " + to_string(levels));
        Multiplier.setString("Multiplier: " + to_string(multiplier));
        window.draw(scoretxt);
        window.draw(highscoretxt);
        window.draw(levelsTxt);
        window.draw(Multiplier);
        window.display();
    }
    window.clear();
    TextTimer.restart();
    Text Outro = Text("Congratulations you have passed the Third boss fight", font, 50);
    Outro.setPosition(Welcome.getPosition().x - 300, Welcome.getPosition().y);

    while (TextTimer.getElapsedTime().asSeconds() < 5) {
        window.clear();
        window.draw(Outro);
        window.display();
    }
    spaceship.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y - 100));
}
void level4Boss(RenderWindow& window, int& levels, Spaceship& spaceship, vector<Bullets>& bullets, int& heart, int& score, int highScore, Picture hearts[], Text& scoretxt, Text& highscoretxt, Text& levelsTxt, Bullets& bullet, Clock& clock, double movement, Asteroid& as, vector<Asteroid>& asteroids, vector<Asteroid>& explodedAsteroids, vector<Clock>& explodedAsteroidsTime, Clock& asteroidClock, int& multiplier, Clock& endMultiplier, Text& Multiplier, string settings[],Sound &HealthLoss,Sound &GameSound,Revive &revive) {
    spaceship.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y - 100));
    double teleportationTimer = 5;
    double bulletSpeed = 0.01;
    setVal(bulletSpeed,teleportationTimer,settings,window);
      SoundBuffer bulletBuffer;
    SoundBuffer asteroidSoundbuffer;
    SoundBuffer BossbulletsBuffer;
    SoundBuffer MinionBuffer;
   if(settings[0]=="0")
    {
        bulletBuffer.loadFromFile("audios/NoSound.wav");
        asteroidSoundbuffer.loadFromFile("audios/NoSound.wav");
        BossbulletsBuffer.loadFromFile("audios/NoSound.wav");
        MinionBuffer.loadFromFile("audios/NoSound.wav");
    }
    else
    {
    if (!bulletBuffer.loadFromFile("audios/BulletShoot.wav")) {
        cout << "Error loading bullet sound" << endl;
    }
    asteroidSoundbuffer.loadFromFile("audios/AsteroidExplosion.wav");
     BossbulletsBuffer.loadFromFile("audios/BossBullet.wav");
     MinionBuffer.loadFromFile("audios/MinionBomb.wav");
    }
    Sound bulletSound(bulletBuffer); 
    Sound BossBulletsSound(BossbulletsBuffer);
    Sound asteroidSound(asteroidSoundbuffer);
    Sound minionStrike(MinionBuffer);

    bulletSound.setBuffer(bulletBuffer);
    int temp = multiplier;
    bool startStrikebool = false;
    AirStrikeMinion minion(window, "MinionShip.png");
    vector<Bullets> minionBullets;
    Bullets minionBullet(window, "Minion.png");
    Clock startStrike;
    Clock minionShoot;
    bool a = false;

    Boss b1(window, "Boss.png");
    Clock bossShoot;
    vector<Bullets> bossBullets;
    Bullets bossBullet(window, "Boss4Bullets.png");
    Font font;
    if (!font.loadFromFile("AGENCYR.ttf")) {
        cout << "Error loading font" << endl;
    }
    Text Welcome = Text("Welcome to the Fourth boss fight", font, 50);
    Welcome.setPosition(window.getSize().x / 2 - 200, window.getSize().y / 2);
    Clock TextTimer;
    while (TextTimer.getElapsedTime().asSeconds() < 5) {
        window.clear();
        window.draw(Welcome);
        window.display();
    }
    window.clear();
    multiplier = temp;
    endMultiplier.restart();
    Clock gameSound;
    GameSound.play();
    while (window.isOpen()) {
        b1.Follow(spaceship, movement);
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        if (event.key.code == Keyboard::Space && (clock.getElapsedTime().asSeconds() > 0.5)) {
            bulletSound.play();
            bullet.SetPosition((spaceship.getPosition().x) + 50, spaceship.getPosition().y - 10);
            bullets.push_back(bullet);
            bullet.drawTo(window);
            window.display();
            clock.restart();

            event.key.code = Keyboard::Unknown;
        }
        if ((Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) && spaceship.checkleft(movement)) {
            spaceship.move(-1 * movement, 0);
        }
        if ((Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) && spaceship.checkright(window, movement)) {
            spaceship.move(movement, 0);
        }
        if ((Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) && spaceship.checkUp(movement, window)) {
            spaceship.move(0, -1 * movement);

        }
        if ((Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) && spaceship.checkdown(window, movement)) {
            spaceship.move(0, movement);

        }

        //This condition will check if the multiplier has ended
        window.clear();
        for (int i = 0; i < bullets.size(); i++) {
            if (bullets[i].getPosition().y < 0) {
                bullets.erase(bullets.begin() + i);
            }
            else {
                bullets[i].move(0, window.getSize().y * -0.02);
                bullets[i].drawTo(window);
            }
        }
        for (int i = 0; i < asteroids.size(); i++) {
            asteroids[i].SetTexture("Asteroid.png");
            asteroids[i].move(window, levels,settings[1]);
            asteroids[i].drawTo(window);
            if (spaceship.getGlobalBounds().intersects(asteroids[i].getGlobalBounds())) {
                HealthLoss.play();
                heart--;
                asteroids.erase(asteroids.begin() + i);
            }
            if (asteroids[i].getPosition().y > window.getSize().y) {
                asteroids.erase(asteroids.begin() + i);
            }
            for (int j = 0; j < bullets.size(); j++) {
                if (asteroids[i].getGlobalBounds().intersects(bullets[j].getGlobalBounds())) {
                    Clock c;
                    asteroidSound.play();
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
                    multiplier++;
                    endMultiplier.restart();
                }
            }
        }
        if (endMultiplier.getElapsedTime().asSeconds() > 3) {
            multiplier = 1;
        }

        if (asteroidClock.getElapsedTime().asSeconds() > 3.0f) {
            as.setRandomPosition(window);
            as.setRandomSize();
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
        //This code is for the minions
        if (startStrike.getElapsedTime().asSeconds() > 5 && !startStrikebool) {
            startStrikebool = true;
            startStrike.restart();
        }
        if (minionShoot.getElapsedTime().asSeconds() > 1 && startStrikebool) {
            minionStrike.play();
            minionBullet.SetPosition(minion.getPosition().x + 50, minion.getPosition().y);
            minionBullets.push_back(minionBullet);
            minionShoot.restart();
        }
        if (minion.getPosition().x < 0) {
            startStrikebool = false;
            startStrike.restart();
            minion.setStartingPosition(window);
        }
        for (int i = 0;i < minionBullets.size();i++) {
            if (minionBullets[i].getGlobalBounds().intersects(spaceship.getGlobalBounds())) {
                HealthLoss.play();
                heart--;
                minionBullets.erase(minionBullets.begin() + i);
            }
            else {
                minionBullets[i].move(0, window.getSize().y * 0.01);
                minionBullets[i].drawTo(window);
            }
        }
        if (startStrikebool) {
            minion.script(window);
        }

        b1.script(window, spaceship, 5);
        //This condition will check if the spaceship bullets has collided with the boss
        for (int i = 0;i < bullets.size();i++) {
            if (bullets[i].getGlobalBounds().intersects(b1.getGlobalBounds())) {
                b1.incrementHits();
                bullets.erase(bullets.begin() + i);
                if (b1.getHits() == 15) {
                    levels++;
                    score += 2000;
                    a = true;
                }
            }
        }
        //This condition will check if the boss bullets has collided with the spaceship
        for (int i = 0;i < bossBullets.size();i++) {
            if (bossBullets[i].getGlobalBounds().intersects(spaceship.getGlobalBounds())) {
                HealthLoss.play();
                heart -= 2;
                bossBullets.erase(bossBullets.begin() + i);

            }
        }
        //This condition will check if the boss bullets are out of bounds
        for (int i = 0;i < bossBullets.size();i++) {
            if (bossBullets[i].getPosition().y > window.getSize().y) {
                bossBullets.erase(bossBullets.begin() + i);
            }
        }
        if (bossShoot.getElapsedTime().asSeconds() > 1) {
            BossBulletsSound.play();
            bossBullet.SetPosition(b1.getPosition().x + 50, b1.getPosition().y + 50);
            bossBullets.push_back(bossBullet);
            bossShoot.restart();
        }
        if (a) {
            break;
        }
        for (int i = 0;i < bossBullets.size();i++) {
            if (bossBullets[i].getPosition().y > 0) {
                bossBullets[i].move(0,bulletSpeed);
                bossBullets[i].drawTo(window);
            }
            else {
                bossBullets.erase(bossBullets.begin() + i);
            }
        }
        spaceship.drawTo(window);
        for (int i = 0; i < heart; i++) {
            hearts[i].drawTo(window);
        }
        if (heart <= 0) {
         revive.SetHealth(heart);
            if(heart<=0)
            {
                return;
            }
            else{
                  continue;
            }
        }
        if(gameSound.getElapsedTime().asSeconds()>GameSound.getBuffer()->getDuration().asSeconds())
        {
            GameSound.play();
            gameSound.restart();
        }
        scoretxt.setString("Score: " + to_string(score));
        highscoretxt.setString("High Score: " + to_string(highScore));
        levelsTxt.setString("Level: " + to_string(levels));
        Multiplier.setString("Multiplier: " + to_string(multiplier));
        window.draw(scoretxt);
        window.draw(highscoretxt);
        window.draw(levelsTxt);
        window.draw(Multiplier);
        window.display();
    }
    window.clear();
    TextTimer.restart();
    Text Outro = Text("Congratulations you have passed the last boss fight", font, 50);
    Outro.setPosition(Welcome.getPosition().x - 300, Welcome.getPosition().y);

    while (TextTimer.getElapsedTime().asSeconds() < 5) {
        window.clear();
        window.draw(Outro);
        window.display();
    }
    spaceship.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y - 100));
}
