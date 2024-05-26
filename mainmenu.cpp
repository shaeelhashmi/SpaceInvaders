#include "spaceship.cpp"

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
    void leftalign(Vector2f pos, float textOffset)
    {
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

void firstScreen(RenderWindow& window)
{
    window.setFramerateLimit(60);

    Texture firstbg;
    if (!firstbg.loadFromFile("bgspace.png"))
    {
        cout << "Failed to load background texture!" << endl;
        return;
    }

    Sprite firstsp(firstbg);

    Button playButton("PLAY", Vector2f(300, 80), 24, Color(141, 26, 22), Color::Black);
    Button optionsButton("OPTIONS", Vector2f(300, 80), 24, Color(141, 26, 22), Color::Black);
    Button quitButton("QUIT", Vector2f(300, 80), 24, Color(141, 26, 22), Color::Black);

    Font font;
    if (!font.loadFromFile("LEMONMILK-Medium.otf")) {
        cout << "Failed to load font!" << endl;
        return;
    }
    playButton.setFont(font);
    optionsButton.setFont(font);
    quitButton.setFont(font);

    Vector2u windowSize = window.getSize();
    playButton.setPosition(Vector2f(windowSize.x / 2 - playButton.getSize().x / 2, 200));
    optionsButton.setPosition(Vector2f(windowSize.x / 2 - optionsButton.getSize().x / 2, 300));
    quitButton.setPosition(Vector2f(windowSize.x / 2 - quitButton.getSize().x / 2, 400));

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                if (playButton.buttonClicked(window))
                {
                    mainmenu(window);
                }
                else if (optionsButton.buttonClicked(window))
                {
                    cout << "Opening options..." << endl;
                }
                else if (quitButton.buttonClicked(window))
                {
                    window.close();
                }
            }
        }

        window.clear();
        window.draw(firstsp);
        playButton.drawTo(window);
        optionsButton.drawTo(window);
        quitButton.drawTo(window);
        window.display();
    }
}

void mainmenu(RenderWindow& window)
{
    window.setFramerateLimit(60);
    string input;

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
    inputText.setPosition(textboxBackground.getPosition().x+10, textboxBackground.getPosition().y+10);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                if (backButton.buttonClicked(window))
                {
                    return;
                }
                if (nextButton.buttonClicked(window)&&!input.empty())
                {
                    game(window, input);
                }
            }
            else if (event.type == Event::TextEntered)
            {
                if (event.text.unicode == 8 && !input.empty()) // Handle backspace
                {
                    input.pop_back();
                }
                else if (event.text.unicode >= 32 && event.text.unicode <= 126) // Handle normal characters
                {
                    if (inputText.getLocalBounds().width + 10 < textboxBackground.getSize().x)
                    {
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