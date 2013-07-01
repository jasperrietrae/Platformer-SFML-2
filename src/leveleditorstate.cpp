#include "leveleditorstate.hpp"

LevelEditorState::LevelEditorState(sf::RenderWindow* renderWindow, StateManager* manager) : m_manager(manager), m_window(renderWindow)
{
    m_levelEditorMenu = new LevelEditorMenu(&m_manager->resourceManager);
    m_levelEditorMenu->button_save.setCallback(&save, this);
    m_levelEditorMenu->button_toggleGrid.setCallback(&toggleGrid, this);
    //m_levelEditorMenu->button_tiles.setCallback(&tiles, this); //! NOT needed (duh :p)

    //m_levelEditorMenu->button_tiles.items.push_back(new Button(sf::Vector2f(20.0f, 100.0f), m_manager->resourceManager.getTexture("Graphics/Menu/block1.png")));
    //m_levelEditorMenu->button_tiles.items.push_back(new Button(sf::Vector2f(120.0f, 100.0f), m_manager->resourceManager.getTexture("Graphics/Menu/block2.png")));
    //m_levelEditorMenu->button_tiles.items.push_back(new Button(sf::Vector2f(220.0f, 100.0f), m_manager->resourceManager.getTexture("Graphics/Menu/block3.png")));

    //for (std::list<std::pair<MenuItem*, std::string> >::iterator itr = m_levelEditorMenu->button_tiles.items.begin(); itr != m_levelEditorMenu->button_tiles.items.end(); ++itr)
    //    if (MenuItem* menuItem = (*itr).first)
    //        ((Button*)menuItem)->setCallback(&setSelectedTile, this, (*itr).second);

    selectedTileFilename = "";
    selectionRespectsGrid = true;
    enabledGrid = true;
    justReselectedTile = false;
    movedCursorOutOfNewTile = true;
    testingLevelOut = false;

    sf::RectangleShape gridShape(sf::Vector2f(50.0f, 50.0f));
    gridShape.setFillColor(sf::Color::Transparent);
    gridShape.setOutlineColor(sf::Color::Black);
    //gridShape.setOutlineColor(sf::Color(255, 255, 255, 10));
    gridShape.setOutlineThickness(1.0f);

    for (int i = 0; i < 12; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            gridShape.setPosition(j * 50.0f, i * 50.0f);
            grid[j][i] = gridShape;
        }
    }
}

void LevelEditorState::handle_events()
{
    sf::Event _event;

    while (m_window->pollEvent(_event))
    {
        m_levelEditorMenu->handle_event(_event);

        switch (_event.type)
        {
            case sf::Event::Closed:
                m_manager->set_next_state(StateManager::GameStates::GAME_STATE_EXIT);
                break;
            case sf::Event::MouseButtonPressed:
            {
                switch (_event.mouseButton.button)
                {
                    case sf::Mouse::Left:
                        MouseButtonPressed(sf::Mouse::getPosition(*m_window), true);
                        break;
                    case sf::Mouse::Right:
                        MouseButtonPressed(sf::Mouse::getPosition(*m_window), false);
                        break;
                }
                break;
            }
            case sf::Event::KeyReleased:
            {
                switch (_event.key.code)
                {
                    case sf::Keyboard::F1:
                    case sf::Keyboard::F2:
                        //! Handled in Unit::Update
                        break;
                    case sf::Keyboard::F3:
                        enabledGrid = !enabledGrid;
                        break;
                    case sf::Keyboard::F4:
                        testingLevelOut = !testingLevelOut;
                        break;
                    case sf::Keyboard::F7:
                        selectedTileFilename = "Graphics/Menu/block1.png";
                        break;
                    case sf::Keyboard::F8:
                        selectedTileFilename = "Graphics/Menu/block2.png";
                        break;
                    case sf::Keyboard::F9:
                        selectedTileFilename = "Graphics/Menu/block3.png";
                        break;
                }
            }
            default:
                break;
        }
    }
}

//! TODO: Make this inlin and spare functions like these in some place...
float GetDistance(float x1, float y1, float x2, float y2)
{
    float dx = x1 - x2;
    float dy = y1 - y2;
    float dist = sqrt((dx * dx) + (dy * dy));
    return (dist > 0 ? dist : 0);
}

bool LevelEditorState::IsSpotTakenBySprite(sf::Vector2f position)
{
    for (std::vector<std::pair<sf::Vector2f, std::string> >::iterator itr = sprites.begin(); itr != sprites.end(); ++itr)
        if ((*itr).first == position)
            return true;

    return false;
}

sf::Vector2f LevelEditorState::GetPositionForSelectedTile()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);

    if (enabledGrid && selectedTileFilename != "" && selectionRespectsGrid)
    {
        if (selectedTileFilename == "Graphics/Menu/collision_pointer.png")
            return sf::Vector2f(float(mousePos.x), float(mousePos.y));

        sf::RectangleShape closestGrid = grid[0][0];

        for (int i = 0; i < 12; ++i)
        {
            for (int j = 0; j < 20; ++j)
            {
                sf::Vector2f gridPos = grid[j][i].getPosition();

                if (IsSpotTakenBySprite(gridPos))
                    continue;

                sf::Vector2f closestGridPos = closestGrid.getPosition();

                //! We add 25.0f because that's 50% of the width and height of the grid block, which means we therefore target the center of that grid
                //! spot so the closestGrid variable only changes when we actually enter a grid block with our cursor.
                if (GetDistance(float(mousePos.x), float(mousePos.y), gridPos.x + 25.0f, gridPos.y + 25.0f) < GetDistance(float(mousePos.x), float(mousePos.y), closestGridPos.x + 25.0f, closestGridPos.y + 25.0f))
                    closestGrid = grid[j][i];
            }
        }

        return closestGrid.getPosition();
    }
    else
        return sf::Vector2f(float(mousePos.x), float(mousePos.y));
}

void LevelEditorState::logic(double passed, double deltaTime)
{
    if (testingLevelOut)
    {
        if (!player)
        {
            sf::RectangleShape bodyShape(sf::Vector2f(75.0f, 75.0f));
            bodyShape.setFillColor(sf::Color::Green);
            player = new Player(m_window, sf::Vector2f(500.0f, 300.0f), bodyShape, m_manager, this);
        }

        player->Update();
    }
}

void LevelEditorState::render(double alpha)
{
    m_window->clear(sf::Color::Cyan);

    sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);

    sf::RectangleShape backgroundImage(sf::Vector2f(1000.0f, 600.0f));
    backgroundImage.setFillColor(sf::Color::Cyan);
    m_window->draw(backgroundImage);

    if (enabledGrid)
        for (int i = 0; i < 12; ++i)
            for (int j = 0; j < 20; ++j)
                m_window->draw(grid[j][i]);

    if (selectedTileFilename != "")
    {
        sf::Sprite selectedTile(m_manager->resourceManager.getTexture(selectedTileFilename));
        selectedTile.setPosition(GetPositionForSelectedTile());
        selectedTile.setColor(sf::Color(255, 255, 255, 100));
        m_window->draw(selectedTile);
    }

    bool foundHoverOverTile = false;

    for (std::vector<std::pair<sf::Vector2f, std::string> >::iterator itr = sprites.begin(); itr != sprites.end(); ++itr)
    {
        sf::Sprite sprite(m_manager->resourceManager.getTexture((*itr).second));
        sprite.setPosition((*itr).first.x, (*itr).first.y);
        sf::FloatRect spriteRect = sprite.getGlobalBounds();

        if (selectedTileFilename == "")
        {
            if (!(mousePos.y >= (*itr).first.y + spriteRect.height || mousePos.x >= (*itr).first.x + spriteRect.width || mousePos.y + 16.0f <= (*itr).first.y || mousePos.x + 16.0f <= (*itr).first.x))
            {
                if (!foundHoverOverTile && ((*itr == sprites.back() && movedCursorOutOfNewTile) || *itr != sprites.back()))
                {
                    foundHoverOverTile = true;
                    sprite.setColor(sf::Color(255, 255, 255, 100));
                }
            }
            else if (*itr == sprites.back() && !movedCursorOutOfNewTile)
                movedCursorOutOfNewTile = true;
        }

        m_window->draw(sprite);
    }

    if (testingLevelOut && player)
    {
        sf::RectangleShape rectShape = player->GetBodyShape();
        rectShape.setPosition(player->GetPosition());
        m_window->draw(rectShape);
    }

    m_window->draw(*m_levelEditorMenu); //! Draw the menu as last part of the level editor
    m_window->display();
}

void LevelEditorState::MouseButtonPressed(sf::Vector2i mousePos, bool leftMouseClick)
{
    if (selectedTileFilename == "" || !leftMouseClick)
    {
        for (std::vector<std::pair<sf::Vector2f, std::string> >::iterator itr = sprites.begin(); itr != sprites.end(); )
        {
            sf::Sprite sprite(m_manager->resourceManager.getTexture((*itr).second));
            sf::FloatRect spriteRect = sprite.getGlobalBounds();

            if (!(mousePos.y >= (*itr).first.y + spriteRect.height || mousePos.x >= (*itr).first.x + spriteRect.width || mousePos.y + 16.0f <= (*itr).first.y || mousePos.x + 16.0f <= (*itr).first.x))
            {
                if (leftMouseClick)
                {
                    justReselectedTile = true;
                    selectedTileFilename = (*itr).second;

                    if ((*itr).second == "Graphics/Menu/collision_pointer.png")
                        selectionRespectsGrid = false;
                }

                itr = sprites.erase(itr);
                break;
            }
            else
                ++itr;
        }
    }

    if (selectedTileFilename != "")
    {
        if (justReselectedTile)
        {
            justReselectedTile = false;
            return;
        }

        movedCursorOutOfNewTile = false;
        sprites.push_back(std::make_pair(GetPositionForSelectedTile(), selectedTileFilename));
        selectedTileFilename = "";
        selectionRespectsGrid = true;
    }
}

void LevelEditorState::save(void* inst, Button* button)
{
    ((LevelEditorState*)inst)->m_manager->set_next_state(StateManager::GameStates::GAME_STATE_EXIT);
}

//void LevelEditorState::tiles(void* inst, CollapsableButton* button)
//{
//    ((LevelEditorState*)inst)->m_levelEditorMenu->button_tiles.collapsed = !self->m_levelEditorMenu->button_tiles.collapsed;
//}

void LevelEditorState::toggleGrid(void* inst, Button* button)
{
    ((LevelEditorState*)inst)->enabledGrid = !((LevelEditorState*)inst)->enabledGrid;
}

void LevelEditorState::setSelectedTile(void* inst, Button* button, std::string filename)
{
    ((LevelEditorState*)inst)->SetSelectedTileFilename(filename);
}
