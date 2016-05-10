//
// Created by Anthony on 5/3/2016.
//

#ifndef ARKANGEL_TERMINAL_HPP
#define ARKANGEL_TERMINAL_HPP

#include <SFGUI/Bin.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

class Terminal : public sfg::Window {
public:
    typedef std::shared_ptr<Terminal> Ptr;
    typedef std::shared_ptr<const Terminal> PtrConst;

    static Ptr Create(char style = sfg::Window::Style::TITLEBAR |
                                   sfg::Window::Style::BACKGROUND |
                                   sfg::Window::Style::RESIZE |
                                   sfg::Window::Style::CLOSE);

    const std::string& GetName() const override;
    void HandleEvent(const sf::Event& event);

    std::string& GetLastCommand();

    void Print(std::string Str);
    void PrintLog(std::string Str);
    void PrintError(std::string Str);

    static sfg::Signal::SignalID OnCommandEntered;

protected:
    sfg::ScrolledWindow::Ptr ScrollWindow;
    sfg::Box::Ptr LogBox;
    sfg::Entry::Ptr Entry;

    std::string LastCommand = "";

    Terminal(char style);

    void EnterCommand();
    void PrintClass(std::string Str, std::string Class);
};


#endif //ARKANGEL_TERMINAL_HPP
