#include "gui.h"

//--------------------------------------------------------------------------------

#define HEXAGON_DX                  0.f
#define HEXAGON_DY                  50.f

#define HEXAGON_SIZE                25.f
#define HEXAGON_OTLINE_THICKNESS    5.f

#define FONT_PATH                   "font.ttf"

#define TEXT_DX                     -18.f
#define TEXT_DY                     -20.f
#define TEXT_SINGLE_DX              10.f

//#define SHOW_CELLS_COORDINATES

GUI::GUI(sint_16 aN, sint_16 aM) :
    mWindow     (sf::VideoMode(aN, aM), "Evolution"),
    mHexagon    (HEXAGON_SIZE, 6)
{
    mHexagon.setOrigin(HEXAGON_SIZE, HEXAGON_SIZE);
    mHexagon.setOutlineColor(sf::Color::White);
    mHexagon.setOutlineThickness(HEXAGON_OTLINE_THICKNESS);

    if (!mFont.loadFromFile(FONT_PATH))
    {
        std::cout << "Font load error!\n";
    }
    mText.setFont(mFont);
    mText.setFillColor(sf::Color::White);
}

bool
GUI::isAppClosed() const
{
    return !mWindow.isOpen();
}

std::vector<GUI::EventType>
GUI::getEvents()
{
    std::vector<EventType> result;
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            mWindow.close();

        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Key::Tab     :
                result.push_back(GUI::EventType::SWITCH_DRAW_MODE);
                break;
            case sf::Keyboard::Key::Space   :
                result.push_back(GUI::EventType::SWITCH_PAUSE);
                break;
            case sf::Keyboard::Key::X       :
                result.push_back(GUI::EventType::INCREASE_SPEED);
                break;
            case sf::Keyboard::Key::Z       :
                result.push_back(GUI::EventType::DECREASE_SPEED);
                break;
            case sf::Keyboard::Key::LShift  :
                result.push_back(GUI::EventType::STANDART_PAUSE);
                break;
            default:
                break;
            }
        }
    }
    return result;
}

void 
GUI::drawField(const std::vector<std::vector<Object*>>& aField)
{
    mWindow.clear(sf::Color::Black);

    for (sint_16 i = 0; i < aField.size(); ++i) {
        for (sint_16 j = 0; j < aField[0].size(); ++j) {
            mHexagon.setPosition(
                (HEXAGON_DX +
                (j % 2 ? (HEXAGON_SIZE * 3) : (HEXAGON_SIZE * 2.))) +
                i * HEXAGON_SIZE * 2.,
                HEXAGON_DY + j * (HEXAGON_SIZE * 1.6f));
            switch (aField[i][j]->getType())
            {
            case Object::ObjectType::VOID   :
                mHexagon.setFillColor(sf::Color::Black);
                break;
            case Object::ObjectType::BOT    :
                mHexagon.setFillColor(sf::Color::Blue);
                break;
            case Object::ObjectType::FOOD   :
                mHexagon.setFillColor(sf::Color::Green);
                break;
            case Object::ObjectType::POISON :
                mHexagon.setFillColor(sf::Color::Red);
                break;
            case Object::ObjectType::WALL   :
                mHexagon.setFillColor(sf::Color(125, 125, 125));
                break;
            default:
                mHexagon.setFillColor(sf::Color(255, 175, 25));
                std::cout << "error gui ";
                break;
            }

            mWindow.draw(mHexagon);

            if (aField[i][j]->getType() == Object::ObjectType::BOT)
            {
                std::string botHealph = std::to_string
                    (static_cast<Bot*>(aField[i][j])->getHealph());
                mText.setPosition(
                    ((botHealph.size() == 1 ? TEXT_SINGLE_DX : 0) + 
                    TEXT_DX + 
                    HEXAGON_DX +
                    (j % 2 ? (HEXAGON_SIZE * 3) : (HEXAGON_SIZE * 2.))) +
                    i * HEXAGON_SIZE * 2.,
                    TEXT_DY + HEXAGON_DY + j * (HEXAGON_SIZE * 1.6f));
                mText.setString(botHealph);
                mWindow.draw(mText);
            }
#ifdef SHOW_CELLS_COORDINATES
            else
            {
                mText.setScale(sf::Vector2f(0.5, 0.5));
                mText.setPosition(
                    (TEXT_DX + HEXAGON_DX +
                        (j % 2 ? (HEXAGON_SIZE * 3) : (HEXAGON_SIZE * 2.))) +
                    i * HEXAGON_SIZE * 2.,
                    TEXT_DY + HEXAGON_DY + j * (HEXAGON_SIZE * 1.6f));
                mText.setString(std::to_string(i) + " " + std::to_string(j));
                mWindow.draw(mText);
                mText.setScale(sf::Vector2f(1, 1));
            }

#endif // SHOW_CELLS_COORDINATES
        }
    }
    mWindow.display();
}

//--------------------------------------------------------------------------------
