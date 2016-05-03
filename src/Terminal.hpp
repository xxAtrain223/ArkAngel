//
// Created by Anthony on 5/3/2016.
//

#ifndef ARKANGEL_TERMINAL_HPP
#define ARKANGEL_TERMINAL_HPP

#include <SFGUI/Bin.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

class SFGUI_API Terminal : public sfg::Window {
public:
    typedef std::shared_ptr<Terminal> Ptr;
    typedef std::shared_ptr<const Terminal> PtrConst;

    static Ptr Create(char style = sfg::Window::Style::TITLEBAR |
                                   sfg::Window::Style::BACKGROUND |
                                   sfg::Window::Style::RESIZE |
                                   sfg::Window::Style::CLOSE);

    void HandleEvent(const sf::Event& event);

protected:
    sfg::ScrolledWindow::Ptr ScrolledWindow;
    sfg::Box::Ptr LogBox;
    sfg::Entry::Ptr Entry;
    sfg::Alignment::Ptr alignment;

    Terminal(char style);

    void EnterCommand();
};


#endif //ARKANGEL_TERMINAL_HPP
