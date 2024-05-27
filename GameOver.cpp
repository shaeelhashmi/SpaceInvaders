#include <SFML/Graphics.hpp>
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
int main() {
    RenderWindow window(VideoMode::getDesktopMode(), "Space Invaders", Style::Close | Style::Fullscreen);
    Button GoTomain("MAIN MENU", Vector2f(300, 80), 24, Color(141, 26, 22), Color::Black);
    Button PlayAgain("PLAY AGAIN", Vector2f(300, 80), 24, Color(141, 26, 22), Color::Black);
    Text GameOver;

    Font font;
    if (!font.loadFromFile("LEMONMILK-Medium.otf")) {
        return 0;
    }
    GameOver = Text("Game Over", font, 50);
    Text Score = Text("Score:1145", font, 20);
    GameOver.setPosition((window.getSize().x / 2.0) - 135, window.getSize().y / 2.0 - 100);
    GoTomain.setFont(font);
    PlayAgain.setFont(font);
    Score.setPosition((window.getSize().x / 2.0) - 60, (window.getSize().y / 2.0));
    GoTomain.setPosition(Vector2f((window.getSize().x / 2.0) - PlayAgain.getSize().x - 5, window.getSize().y / 2 + 100));
    PlayAgain.setPosition(Vector2f((window.getSize().x / 2.0) + 5, window.getSize().y / 2 + 00));
    while (window.isOpen()) {
        window.clear();
        GoTomain.drawTo(window);
        PlayAgain.drawTo((window));
        window.draw(GameOver);
        window.draw(Score);
        window.display();
    }
    return 0;

}