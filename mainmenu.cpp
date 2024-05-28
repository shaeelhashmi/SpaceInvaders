#include "spaceship.cpp"
#include "settings.cpp"
using namespace std;
using namespace sf;
void firstScreen(RenderWindow& window);
void mainmenu(RenderWindow& window);
void displayHighScore(RenderWindow& window) {
    Button Exit("Back to main menu", Vector2f(500, 80), 24, Color(141, 26, 22), Color::Black);
    Button ClearScores("Clear HighScores", Vector2f(500, 80), 24, Color(141, 26, 22), Color::Black);
    vector<Text> highScoresText;
    Font font;
    if (!font.loadFromFile("LEMONMILK-Medium.otf")) {
        cout << "Failed to load font!" << endl;
        return;
    }
    // Vector to store high score s 
    vector<pair<int, string>> highScores;
    // Reading high scores from the file
    ifstream HighScoreInput("highscore.txt");
    if (HighScoreInput.is_open()) {
        string line;
        while (getline(HighScoreInput, line)) {
            size_t pos = line.find(' ');
            if (pos != string::npos) {
                string user = line.substr(0, pos);
                int score = stoi(line.substr(pos + 1));
                highScores.push_back(make_pair(score, user));
            }
        }
        HighScoreInput.close();
    }
    else {
        Exit.leftalign(Vector2f((window.getSize().x / 2) - (Exit.getSize().x / 2) , window.getSize().y / 2 + 40), Exit.getSize().x / 4);
        Exit.setFont(font);
        Text a("No high scores found!", font, 24);
        a.setPosition((window.getSize().x / 2) - 100, window.getSize().y / 2);
        while (window.isOpen()) {

            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    if (Exit.buttonClicked(window)) {
                        window.clear();
                        return;
                    }
                }
            }
            window.clear();
            window.draw(a);
            Exit.drawTo(window);
            window.display();

        }

    }
    sort(highScores.rbegin(), highScores.rend());
    for (int i = 0; i < highScores.size() && i < 5; ++i) {
        highScoresText.push_back(Text(to_string(i + 1) + ". " + highScores[i].second + ": " + to_string(highScores[i].first), font, 24));
    }
    int pos = (window.getSize().y / 2) - (24 * highScoresText.size() / 2);
    for (int i = 0; i < highScoresText.size(); i++) {
        highScoresText[i].setFillColor(Color::White);
        highScoresText[i].setPosition((window.getSize().x / 2) - 50, pos);
        pos += 50;
    }
    Exit.leftalign(Vector2f(10 , pos), Exit.getSize().x / 4);
    ClearScores.leftalign(Vector2f((window.getSize().x )-(ClearScores.getSize().x)-10, pos), Exit.getSize().x / 4);
    Exit.setFont(font);
    ClearScores.setFont(font);
    while (window.isOpen()) {
        window.clear();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (Exit.buttonClicked(window)) {
                    window.clear();
                    return;
                }
                if (ClearScores.buttonClicked(window)) {
                    ofstream HighScoreOutput("highscore.txt",ios::trunc);
                    HighScoreOutput.close();
                    window.clear();
                    return;
            }
            
        }

        for (int i = 0;i < highScoresText.size();i++) {
            window.draw(highScoresText[i]);
        }
        Exit.drawTo(window);
        ClearScores.drawTo(window);
        window.display();
    }
}
}
void firstScreen(RenderWindow& window) {
    window.setFramerateLimit(60);
    ifstream settingsFile("settings.txt");
    string settings[3];
     if (settingsFile.is_open()) {
        string line;
        int i = 0;
        while (getline(settingsFile, line)) {
            settings[i++] = line;
        }
    }
    SoundBuffer menuBuffer;
    if(settings[0] == "0"){
       menuBuffer.loadFromFile("audios/NoSound.wav");
    }
    else{
        menuBuffer.loadFromFile("audios/Mainmenu.wav");
    }  
    Sound menuMusic;    
    menuMusic.setBuffer(menuBuffer);
    Texture firstbg;
    if (!firstbg.loadFromFile("bgspace.png")) {
        cout << "Failed to load background texture!" << endl;
        return;
    }

    Sprite firstsp(firstbg);

    Button playButton("PLAY", Vector2f(300, 80), 24, Color(141, 26, 22), Color::Black);
    Button quitButton("QUIT", Vector2f(300, 80), 24, Color(141, 26, 22), Color::Black);
    Button Highscore("HIGH SCORE", Vector2f(300, 80), 24, Color(141, 26, 22), Color::Black);
    Button Settings("SETTINGS", Vector2f(300, 80), 24, Color(141, 26, 22), Color::Black);
    Font font;
    if (!font.loadFromFile("LEMONMILK-Medium.otf")) {
        cout << "Failed to load font!" << endl;
        return;
    }
    playButton.setFont(font);
    quitButton.setFont(font);
    Highscore.setFont(font);
    Settings.setFont(font);
    Vector2u windowSize = window.getSize();
    playButton.setPosition(Vector2f(windowSize.x / 2 - playButton.getSize().x / 2, 200));
    quitButton.setPosition(Vector2f(windowSize.x / 2 - quitButton.getSize().x / 2, 500));
    Highscore.setPosition(Vector2f(windowSize.x / 2 - Highscore.getSize().x / 2, 300));
    Settings.setPosition(Vector2f(windowSize.x / 2 - Settings.getSize().x / 2, 400));
    menuMusic.play();
    Clock clock;
    while (window.isOpen()) {
        if(clock.getElapsedTime().asSeconds() > menuMusic.getBuffer()->getDuration().asSeconds()){
            menuMusic.play();
            clock.restart();
        }
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (playButton.buttonClicked(window)) {
                    menuMusic.stop();
                    mainmenu(window);
                }
                else if (quitButton.buttonClicked(window)) {
                    menuMusic.stop();
                    window.close();
                }
                else if (Highscore.buttonClicked(window)) {
                    menuMusic.stop();
                    displayHighScore(window);
                }
                else if (Settings.buttonClicked(window)) {
                    menuMusic.stop();
                settingsScreen(window);
                ifstream settingsFile("settings.txt");
                if (settingsFile.is_open()) {
                string line;
                int i = 0;
                while (getline(settingsFile, line)) {
                settings[i++] = line;
                }
            }
            if(settings[0] == "0"){
            menuBuffer.loadFromFile("audios/NoSound.wav");
            }
            else{
                menuBuffer.loadFromFile("audios/Mainmenu.wav");
            }    
            menuMusic.setBuffer(menuBuffer);
            menuMusic.play();
            }
            }
        }

        window.clear();
        window.draw(firstsp);
        playButton.drawTo(window);
        quitButton.drawTo(window);
        Highscore.drawTo(window);
        Settings.drawTo(window);
        window.display();
    }
}

void mainmenu(RenderWindow& window) {
    window.setFramerateLimit(60);
    string input;
    string settings[3];

    Button backButton("back", Vector2f(205, 50), 24, Color(141, 26, 22), Color::Black);
    Button nextButton("next", Vector2f(205, 50), 24, Color(141, 26, 22), Color::Black);

    Font font;
    if (!font.loadFromFile("LEMONMILK-Medium.otf")) {
        cout << "Failed to load font!" << endl;
        return;
    }
    backButton.setFont(font);
    nextButton.setFont(font);

    Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(25);
    inputText.setFillColor(Color::Black);

    RectangleShape textboxBackground;
    textboxBackground.setSize(Vector2f(590.f, 50.f));
    textboxBackground.setFillColor(Color::White);

    backButton.setPosition(Vector2f(200, 520));
    nextButton.setPosition(Vector2f(850, 520));



    Text name("name:", font, 17);
    name.setFillColor(Color::White);
    name.setPosition(356.f, 199.f);

    textboxBackground.setPosition((window.getSize().x / 2.0) - (textboxBackground.getSize().x / 2.0), window.getSize().y / 3.0);
    inputText.setPosition(textboxBackground.getPosition().x + 10, textboxBackground.getPosition().y + 10);
    
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (backButton.buttonClicked(window)) {
                    return;
                }
                if (nextButton.buttonClicked(window) && !input.empty()) {
                    game(window, input);
                    return;
                }
            }
            else if (event.type == Event::TextEntered) {
                if (event.text.unicode == 8 && !input.empty()) // Handle backspace
                {
                    input.pop_back();
                }

                else if (event.text.unicode >= 32 && event.text.unicode <= 126 && event.text.unicode != ' ') // Handle normal characters
                {
                    if (inputText.getLocalBounds().width + 10 < textboxBackground.getSize().x) {
                        input += static_cast<char>(event.text.unicode);
                    }
                }
                inputText.setString(input);
            }
        }

        window.clear(Color::Black);
        window.draw(textboxBackground);
        window.draw(inputText);
        window.draw(name);
        backButton.drawTo(window);
        nextButton.drawTo(window);
        window.display();
    }
}

int main() {
    RenderWindow window(VideoMode::getDesktopMode(), "Space Invaders", Style::Close | Style::Fullscreen);
    firstScreen(window);
    return 0;
}