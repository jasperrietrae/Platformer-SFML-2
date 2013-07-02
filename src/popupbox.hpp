#ifndef POPUPBOX_HPP_INCLUDED
#define POPUPBOX_HPP_INCLUDED

#include <string>

#include "button.hpp"
#include "statemanager.hpp"
#include "popupboxmenu.hpp"

class PopUpBox
{
    public:
        PopUpBox(sf::RenderWindow* renderWindow, StateManager* manager, std::string _popUpBoxMessage, sf::Vector2f position);
        ~PopUpBox();

        void handle_events();
        void logic(double passed, double deltaTime);
        void render(double alpha);

        static void yes(void* inst, Button* button);
        static void no(void* inst, Button* button);

        bool m_pressedYes, m_pressedNo;

    private:
        StateManager* m_manager;
        sf::Text popUpBoxMessage;
        sf::RectangleShape menuBackground;
        sf::RenderWindow* m_window;
        PopUpBoxMenu* m_popUpBoxMenu;
};


#endif // POPUPBOX_HPP_INCLUDED
