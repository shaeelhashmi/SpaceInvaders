using namespace std;
using namespace sf;

void writeToLine(string filename, int lineNumber, string newContent) {
    ifstream inFile(filename);
    vector<string> lines;
    string line;

    while (getline(inFile, line)) {
        lines.push_back(line);
    }
    inFile.close();

    lines[lineNumber - 1] = newContent;

    ofstream outFile(filename);

    for (const auto& l : lines) {
        outFile << l << endl;
    }
    outFile.close();
}

void settingsScreen(RenderWindow& window,Sound &sound) {
    window.setFramerateLimit(30);
    Font font;
    Picture checkMark("checkmark.png");
    ifstream file("settings.txt");
    int difficultySelected, frameRateSelected;
    bool soundEnabled;
    file >> soundEnabled;
    Button headings[3] = { Button("Frame Rate", Vector2f(60, 30), 35, Color::Transparent, Color::White), Button("Difficulty", Vector2f(60, 30), 35, Color::Transparent, Color::White), Button("Sound", Vector2f(60, 30), 35, Color::Transparent, Color::White) };
    Button FPS[3] = { Button("30", Vector2f(80, 60), 20, Color::Black, Color::White), Button("60", Vector2f(80, 60), 20, Color::White, Color::White), Button("90", Vector2f(80, 60), 20, Color::White, Color::White) };
    Button Difficulty[3] = { Button("Easy", Vector2f(80, 60), 12, Color::White, Color::White), Button("Medium", Vector2f(80, 60), 12, Color::Black, Color::White), Button("Hard", Vector2f(80, 60), 12, Color::Black, Color::White) };
    Button soundCheckbox("", Vector2f(40, 40), 0, Color::Black, Color::Transparent);
    Button OK("OK", Vector2f(100, 60), 20, Color::Black, Color::White);
    string options[3];
    
    Texture settingsBg;
    if (!settingsBg.loadFromFile("bgspace.png")) {
        cout << "Failed to load background texture!" << endl;
        return;
    }
    Sprite settingsSp(settingsBg);

    if (!font.loadFromFile("BuckBoard.ttf")) {
        cout << "Failed to load font!" << endl;
        return;
    }
    soundCheckbox.setPosition(Vector2f(790, 410));
    checkMark.setPosition(Vector2f(790, 410));
    checkMark.setScale(Vector2f(40, 40));
    OK.setFont(font);
    OK.setPosition(Vector2f(660, 600));
    Difficulty[2].getText().Bold;
    for (int i = 0, y = 205; i < 3; i++) {
        headings[i].setFont(font);
        headings[i].setPosition(Vector2f(600, y));
        y += 100;
    }
    for (int i = 0, x = 800; i < 3; i++) {
        FPS[i].setFont(font);
        FPS[i].setPosition(Vector2f(x, 200));
        x += 80;
    }
    for (int i = 0, x = 800; i < 3; i++) {
        Difficulty[i].setFont(font);
        Difficulty[i].setPosition(Vector2f(x, 300));
        x += 80;
    }
    sound.play();
    Clock clock;
    while (window.isOpen()) {
      if(clock.getElapsedTime().asSeconds() > sound.getBuffer()->getDuration().asSeconds()){
            sound.play();
            clock.restart();
        }
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (FPS[0].buttonClicked(window)) {
                    writeToLine("settings.txt", 3, "30");
                    frameRateSelected = 0;
                }
                else if (FPS[1].buttonClicked(window)) {
                    writeToLine("settings.txt", 3, "60");
                    frameRateSelected = 1;
                }
                else if (FPS[2].buttonClicked(window)) {
                    writeToLine("settings.txt", 3, "90");
                    frameRateSelected = 2;
                }
                if (Difficulty[0].buttonClicked(window)) {
                    writeToLine("settings.txt", 2, "easy");
                    difficultySelected = 0;
                }
                else if (Difficulty[1].buttonClicked(window)) {
                    writeToLine("settings.txt", 2, "medium");
                    difficultySelected = 1;
                }
                else if (Difficulty[2].buttonClicked(window)) {
                    writeToLine("settings.txt", 2, "hard");
                    difficultySelected = 2;
                }
                if (soundCheckbox.buttonClicked(window) && soundEnabled) {
                    soundEnabled = false;
                    writeToLine("settings.txt", 1, "0");
                }
                else if (soundCheckbox.buttonClicked(window) && !soundEnabled) {
                    soundEnabled = true;
                    writeToLine("settings.txt", 1, "1");
                }
                if (OK.buttonClicked(window)) {
                    return;
                }
            }
        }
        for (int i = 0; i < 3; i++) {
            Difficulty[i].setBackColor(Color::Black);
            FPS[i].setBackColor(Color::Black);
        }
        for (int i = 0; i < 3; i++) {
            getline(file, options[i]);
        }
        
        window.clear();
        window.draw(settingsSp);
        for (int i = 0; i < 3; i++) {

            if (difficultySelected == i) {
                Difficulty[i].setBackColor(Color(77,80,63));
            }
            if (frameRateSelected == i) {
                FPS[i].setBackColor(Color(77,80,63));
            }

            headings[i].drawTo(window);
            FPS[i].drawTo(window);
            Difficulty[i].drawTo(window);
        }
        soundCheckbox.drawTo(window);
        if (soundEnabled) {
            checkMark.drawTo(window);
        }
        OK.drawTo(window);
        window.display();
    }
}
