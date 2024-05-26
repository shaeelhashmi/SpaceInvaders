#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include "GameClasses.cpp"
using namespace std;
using namespace sf;

void firstScreen(RenderWindow& window);
void mainmenu(RenderWindow& window);

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
    private:
    RectangleShape button;
    Text text;
    float offset;
};

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

void settingsScreen(RenderWindow& window) {
    window.setFramerateLimit(30);

    Texture firstbg;
    Sprite firstsp(firstbg);
    Font font;
    Picture checkMark("checkmark.png");
    ifstream file("settings.txt");
    int difficultySelected, frameRateSelected;
    bool soundEnabled;
    file >> soundEnabled;

    Button headings[3] = { Button("Frame Rate", Vector2f(50, 30), 40, Color::Transparent, Color::White), Button("Difficulty", Vector2f(50, 30), 40, Color::Transparent, Color::White), Button("Sound", Vector2f(50, 30), 40, Color::Transparent, Color::White) };
    Button FPS[3] = { Button("30", Vector2f(70, 50), 20, Color::Blue, Color::Black), Button("60", Vector2f(70, 50), 20, Color::Blue, Color::Black), Button("90", Vector2f(70, 50), 20, Color::Blue, Color::Black) };
    Button Difficulty[3] = { Button("Easy", Vector2f(70, 50), 15, Color::Blue, Color::Black), Button("Medium", Vector2f(70, 50), 15, Color::Blue, Color::Black), Button("Hard", Vector2f(70, 50), 15, Color::Blue, Color::Black) };
    Button soundCheckbox("", Vector2f(40, 40), 0, Color::White, Color::Transparent);
    Button OK("OK", Vector2f(100, 50), 20, Color::Blue, Color::Black);
    string options[3];

    if (!firstbg.loadFromFile("bgspace.png")) {
        cout << "Failed to load background texture!" << endl;
        return;
    }


    if (!font.loadFromFile("LEMONMILK-Medium.otf")) {
        cout << "Failed to load font!" << endl;
        return;
    }


    soundCheckbox.setPosition(Vector2f(790, 410));
    checkMark.setPosition(Vector2f(790, 410));
    checkMark.setScale(Vector2f(40, 40));
    OK.setFont(font);
    OK.setPosition(Vector2f(650, 500));

    Difficulty[2].getText().Bold;

    for (int i = 0, y = 205; i < 3; i++) {
        headings[i].setFont(font);
        headings[i].setPosition(Vector2f(500, y));
        y += 100;
    }
    for (int i = 0, x = 700; i < 3; i++) {
        FPS[i].setFont(font);
        FPS[i].setPosition(Vector2f(x, 300));
        x += 80;
    }
    for (int i = 0, x = 700; i < 3; i++) {
        Difficulty[i].setFont(font);
        Difficulty[i].setPosition(Vector2f(x, 200));
        x += 80;
    }
    while (window.isOpen()) {
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
            Difficulty[i].setBackColor(Color::Blue);
            FPS[i].setBackColor(Color::Blue);
        }
        for (int i = 0; i < 3; i++) {
            getline(file, options[i]);
        }
        window.clear();
        for (int i = 0; i < 3; i++) {

            if (difficultySelected == i) {
                Difficulty[i].setBackColor(Color::Yellow);
            }
            if (frameRateSelected == i) {
                FPS[i].setBackColor(Color::Yellow);
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
int main() {
    RenderWindow window(VideoMode::getDesktopMode(), "Space Invaders", Style::Close | Style::Fullscreen);
    settingsScreen(window);
    return 0;
}
