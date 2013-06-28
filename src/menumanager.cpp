#include <iostream>

#include <SFML/Graphics.hpp>

#include "menubutton.hpp"
#include "menumanager.hpp"
#include "statemanager.hpp"

MenuManager::MenuManager(sf::RenderWindow* renderWindow, StateManager* manager)
{
    m_manager = manager;
    m_window = renderWindow;

    nextMenuState = MENU_STATE_NONE;
    currMenuState = MENU_STATE_MAIN;
    prevMenuState = MENU_STATE_MAIN;

    LoadMenus();
}

void MenuManager::LoadMenus()
{
    //! Main Menu
    std::vector<MenuButton*> menuButtons;

    menuButtons.push_back(new MenuButton(0, "Graphics/Menu/back.png", sf::Vector2f(100.0f, 0.0f)));
    menuButtons.push_back(new MenuButton(1, "Graphics/Menu/play.png", sf::Vector2f(100.0f, 100.0f)));
    //! Temporarily disabled the child buttons because we don't need them at the moment. They'll become useful when we
    //! start working on stuff like a level editor.
    //MenuButton* menuButtonMain1 = new MenuButton(0, "Graphics/Menu/options.png", sf::Vector2f(100.0f, 200.0f));
    //menuButtonMain1->AddChildButton(new MenuButton(0, "Graphics/Menu/volume.png", sf::Vector2f(300.0f, 100.0f)));
    //menuButtonMain1->AddChildButton(new MenuButton(1, "Graphics/Menu/herp.png", sf::Vector2f(300.0f, 150.0f)));
    //menuButtonMain1->AddChildButton(new MenuButton(2, "Graphics/Menu/derp.png", sf::Vector2f(300.0f, 200.0f)));
    menuButtons.push_back(new MenuButton(2, "Graphics/Menu/options.png", sf::Vector2f(100.0f, 200.0f)));
    menuButtons.push_back(new MenuButton(3, "Graphics/Menu/quit.png", sf::Vector2f(100.0f, 300.0f)));
    menus[MENU_STATE_MAIN] = menuButtons; //! Play, Options, Quit

    //! Level selection sub-menu
    menuButtons.clear();

    menuButtons.push_back(new MenuButton(0, "Graphics/Menu/back.png", sf::Vector2f(100.0f, 0.0f)));
    menuButtons.push_back(new MenuButton(1, "Graphics/Menu/level_1.png", sf::Vector2f(100.0f, 100.0f)));
    menuButtons.push_back(new MenuButton(2, "Graphics/Menu/level_2.png", sf::Vector2f(100.0f, 200.0f)));
    menuButtons.push_back(new MenuButton(3, "Graphics/Menu/level_3.png", sf::Vector2f(100.0f, 300.0f)));
    menus[MENU_STATE_LEVEL_SELECTION] = menuButtons; //! Levels 1, 2, 3

    //! Options sub-menu
    menuButtons.clear();

    menuButtons.push_back(new MenuButton(0, "Graphics/Menu/back.png", sf::Vector2f(100.0f, 0.0f)));
    menuButtons.push_back(new MenuButton(1, "Graphics/Menu/volume.png", sf::Vector2f(100.0f, 100.0f)));
    menuButtons.push_back(new MenuButton(2, "Graphics/Menu/herp.png", sf::Vector2f(100.0f, 200.0f)));
    menuButtons.push_back(new MenuButton(3, "Graphics/Menu/derp.png", sf::Vector2f(100.0f, 300.0f)));
    menus[MENU_STATE_OPTIONS] = menuButtons; //! Volume, Herp, Derp
}

void MenuManager::MouseButtonPressed(sf::Vector2i mousePos)
{
    if (currMenuState == MENU_STATE_NONE)
    {
        std::cout << "MenuManager::MouseButtonPressed was called with MenuManager::currMenuState set to MENU_STATE_NONE." << std::endl;
        return;
    }

    for (std::vector<MenuButton*>::iterator itr = menus[currMenuState].begin(); itr != menus[currMenuState].end(); ++itr)
    {
        sf::FloatRect buttonRect = sf::Sprite(m_manager->resourceManager.getTexture((*itr)->GetTextureFilename())).getGlobalBounds();

        if (!(mousePos.y >= (*itr)->GetPositionY() + buttonRect.height || mousePos.x >= (*itr)->GetPositionX() + buttonRect.width || mousePos.y + 16.0f <= (*itr)->GetPositionY() || mousePos.x + 16.0f <= (*itr)->GetPositionX()))
        {
            //! This will make the child buttons roll out (if any).
            if ((*itr)->HasChildButtons())
                (*itr)->SetIsCollapsed(!(*itr)->IsCollapsed());

            switch (currMenuState)
            {
                case MENU_STATE_MAIN:
                {
                    switch ((*itr)->GetButtonId())
                    {
                        case 0: //! Back
                        {
                            nextMenuState = prevMenuState;
                            break;
                        }
                        case 1: //! Play Game
                        {
                            nextMenuState = MENU_STATE_LEVEL_SELECTION;
                            break;
                        }
                        case 2: //! Options
                        {
                            nextMenuState = MENU_STATE_OPTIONS;
                            break;
                        }
                        case 3: //! Exit Game(tell the statemanager to shut down)
                        {
                            m_manager->set_next_state(StateManager::GameStates::GAME_STATE_EXIT);
                            break;
                        }
                        default:
                            std::cout << "MenuManager::MouseButtonPressed: Unsupported button id " << (*itr)->GetButtonId() << ", menu " << currMenuState << std::endl;
                            break;
                    }
                    break;
                }
                case MENU_STATE_OPTIONS:
                {
                    switch ((*itr)->GetButtonId())
                    {
                        case 0: //! Back
                        {
                            nextMenuState = prevMenuState;
                            break;
                        }
                        case 1: //! Volume
                        {
                            std::cout << "Volume" << std::endl;
                            break;
                        }
                        case 2: //! Herp
                        {
                            std::cout << "Herp" << std::endl;
                            break;
                        }
                        case 3: //! Derp
                        {
                            std::cout << "Derp" << std::endl;
                            break;
                        }
                        default:
                            std::cout << "MenuManager::MouseButtonPressed: Unsupported button id " << (*itr)->GetButtonId() << ", menu " << currMenuState << std::endl;
                            break;
                    }
                    break;
                }
                case MENU_STATE_LEVEL_SELECTION:
                {
                    switch ((*itr)->GetButtonId())
                    {
                        case 0: //! Back
                        {
                            nextMenuState = prevMenuState;
                            break;
                        }
                        case 1: //! Level 1
                        {
                            m_manager->set_next_state(StateManager::GameStates::GAME_STATE_GAME);
                            break;
                        }
                        case 2: //! Level 2
                        {
                            m_manager->set_next_state(StateManager::GameStates::GAME_STATE_GAME);
                            break;
                        }
                        case 3: //! Level 3
                        {
                            m_manager->set_next_state(StateManager::GameStates::GAME_STATE_GAME);
                            break;
                        }
                        default:
                            std::cout << "MenuManager::MouseButtonPressed: Unsupported button id " << (*itr)->GetButtonId() << ", menu " << currMenuState << std::endl;
                            break;
                    }
                    break;
                }
                default:
                    std::cout << "MenuManager::MouseButtonPressed: Unsupported menustate " << currMenuState << std::endl;
                    break;
            }

            break;
        }
    }
}

void MenuManager::handle_events()
{
    sf::Event _event;

    while (m_window->pollEvent(_event))
    {
        switch (_event.type)
        {
            case sf::Event::Closed:
            {
                //Set the manager to shut down
                m_manager->set_next_state(StateManager::GameStates::GAME_STATE_EXIT);
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                switch (_event.mouseButton.button)
                {
                    case sf::Mouse::Left:
                        MouseButtonPressed(sf::Mouse::getPosition(*m_window));
                        break;
                    case sf::Mouse::Right:
                        break;
                }
                break;
            }
            case sf::Event::KeyReleased:
            {
                switch (_event.key.code)
                {
                    case sf::Keyboard::F4:
                        nextMenuState = prevMenuState;
                        break;
                }
            }
            default:
                break;
        }
    }
}

void MenuManager::logic(double passed, double deltaTime)
{
    if (currMenuState == MENU_STATE_NONE)
    {
        std::cout << "MenuManager::Update was called with MenuManager::currMenuState set to MENU_STATE_NONE." << std::endl;
        return;
    }

    if (nextMenuState != MENU_STATE_NONE)
    {
        prevMenuState = currMenuState;
        currMenuState = nextMenuState;
        nextMenuState = MENU_STATE_NONE;
    }

    switch (currMenuState)
    {
        case MENU_STATE_MAIN:
        {
            break;
        }
        case MENU_STATE_OPTIONS:
        {
            break;
        }
        case MENU_STATE_LEVEL_SELECTION:
        {
            break;
        }
        default:
            std::cout << "MenuManager::Update: Unsupported menustate " << currMenuState << std::endl;
            break;
    }
}

void MenuManager::render(double alpha)
{
    m_window->clear();

    for (std::vector<MenuButton*>::iterator itr = menus[currMenuState].begin(); itr != menus[currMenuState].end(); ++itr)
    {
        sf::Sprite spriteButton(m_manager->resourceManager.getTexture((*itr)->GetTextureFilename()));

        //! We center the images by substracting 50% of the width from the X position and 50% of the height from the Y position.
        spriteButton.setPosition((*itr)->GetPositionX() - spriteButton.getGlobalBounds().width / 2.0f, (*itr)->GetPositionY() - spriteButton.getGlobalBounds().height / 2.0f);
        m_window->draw(spriteButton);

        if ((*itr)->HasChildButtons() && (*itr)->IsCollapsed())
        {
            for (std::vector<MenuButton*>::iterator itr2 = (*itr)->GetChildButtons().begin(); itr2 != (*itr)->GetChildButtons().end(); ++itr2)
            {
                sf::Sprite childButton(m_manager->resourceManager.getTexture((*itr2)->GetTextureFilename()));
                childButton.setPosition((*itr2)->GetPositionX() - childButton.getGlobalBounds().width / 2.0f, (*itr2)->GetPositionY() - childButton.getGlobalBounds().height / 2.0f);
                m_window->draw(childButton);
            }
        }
    }

    m_window->display();
}
