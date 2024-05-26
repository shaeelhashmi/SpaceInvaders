#include "bossFight.cpp"

using namespace std;
using namespace sf;

void game(RenderWindow& window, string username) {
    Spaceship spaceship(window);
    Bullets bullet(window, "bullets.png");
    Picture hearts[5] = { Picture("hearts.png"), Picture("hearts.png"), Picture("hearts.png"), Picture("hearts.png"), Picture("hearts.png") };
    Asteroid as(window, "Asteroid.png", spaceship.getSize().x);
    Boss b1(window, "Spaceship.png");
    string settings[3];

    int levels = 1;
    int shootedAsteroids = 0;

    Clock clock;
    // The clock for the multiplier to end
    Clock endMultiplier;
    Clock asteroidClock;

    vector<Bullets> bullets;
    vector<Asteroid> asteroids;

    // This array will store the exploded array
    vector<Asteroid> explodedAsteroids;

    // This vector will store the time for each exploded asteroid
    vector<Clock> explodedAsteroidsTime;

    // This will be increased for every asteroid destroyed
    bool changelevel = false;
    int heart = 5;
    int multiplier = 1;
    bool a = true;
    double movement = window.getSize().x * window.getSize().y * 0.0000039 * 2;
    int score = 0;
    int highScore = 0;
    vector<pair<int, string>> highScores; // Pair to store high scores with usernames

    ifstream settingsFile("settings.txt");
    ifstream HighScoreInput("highscore.txt");
    if (HighScoreInput.is_open()) {
        string line;
        while (getline(HighScoreInput, line)) {
            size_t pos = line.find(' ');
            if (pos != string::npos) {
                string user = line.substr(0, pos);
                int userScore = stoi(line.substr(pos + 1));
                highScores.push_back(make_pair(userScore, user));
            }
        }
        HighScoreInput.close();

        if (settingsFile.is_open()) {
            string line;
            int i = 0;
            while (getline(settingsFile, line)) {
                settings[i++] = line;
            }
        }
        HighScoreInput.close();

        // Sort high scores in descending order
        sort(highScores.rbegin(), highScores.rend());
        if (!highScores.empty()) {
            highScore = highScores[0].first;
        }
    }
    else {
        cout << "File is not open" << endl;
    }

    Font font;
    if (!font.loadFromFile("AGENCYR.ttf")) {
        cout << "Error loading font" << endl;
    }
    Text scoretxt, highscoretxt, levelsTxt;
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
        window.setFramerateLimit(stoi(settings[2]));
        if (score > highScore) {
            highScore = score;
        }
        // These conditions will start the boss levels
        if (levels == 5) {
            changelevel = true;
            if (asteroids.size() == 0) {
                level1Boss(window, levels, spaceship, bullets, heart, score, highScore, hearts, scoretxt, highscoretxt, levelsTxt, bullet, clock, movement);
                changelevel = false;
            }
        }
        else if (levels == 10) {
            changelevel = true;
            if (asteroids.size() == 0) {
                level2Boss(window, levels, spaceship, bullets, heart, score, highScore, hearts, scoretxt, highscoretxt, levelsTxt, bullet, clock, movement);
                changelevel = false;
            }
        }
        else if (levels == 15) {
            changelevel = true;
            if (asteroids.size() == 0) {
                level3Boss(window, levels, spaceship, bullets, heart, score, highScore, hearts, scoretxt, highscoretxt, levelsTxt, bullet, clock, movement, as, asteroids, explodedAsteroids, explodedAsteroidsTime, asteroidClock, multiplier, endMultiplier);
                changelevel = false;
            }
        }
        else if (levels == 20) {
            changelevel = true;
            if (asteroids.size() == 0) {
                level4Boss(window, levels, spaceship, bullets, heart, score, highScore, hearts, scoretxt, highscoretxt, levelsTxt, bullet, clock, movement, as, asteroids, explodedAsteroids, explodedAsteroidsTime, asteroidClock, multiplier, endMultiplier);
                changelevel = false;
            }
        }
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        if (event.key.code == Keyboard::Space && (clock.getElapsedTime().asSeconds() > 1 || a)) {
            bullet.SetPosition((spaceship.getPosition().x) + 50, spaceship.getPosition().y - 10);
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
        if ((Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) && spaceship.checkUp(movement, window)) {
            spaceship.move(0, -1 * movement);
        }
        if ((Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) && spaceship.checkdown(window, movement)) {
            spaceship.move(0, movement);
        }
        // This condition will check if the multiplier has ended
        if (endMultiplier.getElapsedTime().asSeconds() > 3) {
            multiplier = 1;
        }
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
            asteroids[i].move(window);
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
        if (shootedAsteroids == 5) {
            levels++;
            shootedAsteroids = 0;
        }
        if (asteroidClock.getElapsedTime().asSeconds() > 3.0f) {
            if (!changelevel) {
                as.setRandomPosition(window);
                as.setRandomSize();
                shootedAsteroids++;
                asteroids.push_back(as);
                asteroidClock.restart();
            }
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
        spaceship.drawTo(window);
        for (int i = 0; i < heart; i++) {
            hearts[i].drawTo(window);
        }
        // Ending the game
        if (heart <= 0) {
            cout << "GAME OVER" << endl;

            // Update high scores
            highScores.push_back(make_pair(score, username));
            sort(highScores.rbegin(), highScores.rend());
            if (highScores.size() > 5) {
                highScores.resize(5); // Keep only top 5 scores
            }

            ofstream HighScoreOutput("highscore.txt");
            for (const auto& entry : highScores) {
                HighScoreOutput << entry.second << " " << entry.first << endl;
            }
            HighScoreOutput.close();

            window.close();
        }
        scoretxt.setString("Score: " + to_string(score));
        highscoretxt.setString("High Score: " + to_string(highScore));
        levelsTxt.setString("Level: " + to_string(levels));
        window.draw(scoretxt);
        window.draw(highscoretxt);
        window.draw(levelsTxt);
        window.display();
    }
}
int main() {
    RenderWindow window(VideoMode::getDesktopMode(), "Space Invaders", Style::Close | Style::Fullscreen);
    game(window, "Mujtaba");
}
