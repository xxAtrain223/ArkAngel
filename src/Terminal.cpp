//
// Created by Anthony on 5/3/2016.
//

#include "Terminal.hpp"

#include <iostream>

using namespace std;

Terminal::Terminal(char style) : Window(style)
{
}

Terminal::Ptr Terminal::Create(char style)
{
    Terminal::Ptr terminal( new Terminal( style ) );

    terminal->SetTitle("Terminal");
    terminal->GetSignal(sfg::Window::OnCloseButton).Connect(std::bind([&]{ terminal->Show(false); }));

    terminal->LogBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

    terminal->ScrollWindow = sfg::ScrolledWindow::Create();
    terminal->ScrollWindow->SetScrollbarPolicy(sfg::ScrolledWindow::ScrollbarPolicy::HORIZONTAL_AUTOMATIC |
                                       sfg::ScrolledWindow::ScrollbarPolicy::VERTICAL_AUTOMATIC);
    terminal->ScrollWindow->AddWithViewport(terminal->LogBox);
    terminal->ScrollWindow->SetRequisition(sf::Vector2f(400.f, 200.f));

    auto entryBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);

    terminal->Entry = sfg::Entry::Create();
    terminal->Entry->SetRequisition(sf::Vector2f(400.f, 0.f));

    auto button = sfg::Button::Create("Submit");
    button->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&Terminal::EnterCommand, terminal));

    entryBox->Pack(terminal->Entry, true, true);
    entryBox->Pack(button, false, true);

    auto windowBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
    windowBox->Pack(entryBox, false, true);
    windowBox->Pack(terminal->ScrollWindow, true, true);

    terminal->Add(windowBox);

    terminal->RequestResize();
    return terminal;
}

void Terminal::HandleEvent(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::KeyPressed:
            switch(event.key.code)
            {
                case sf::Keyboard::Key::Return:
                    EnterCommand();
                    break;
            }
            break;
    }

    sfg::Window::HandleEvent(event);
}

void Terminal::EnterCommand()
{
    const string text = Entry->GetText();
    if (text.length() > 0) {
        auto label = sfg::Label::Create(text);
        label->SetAlignment(sf::Vector2f(0.f, 0.f));
        LogBox->PackStart(label);

        LastCommand = text;
        GetSignals().Emit(OnCommandEntered);
    }
    Entry->SetText("");
    Entry->GrabFocus();
}

std::string& Terminal::GetLastCommand() {
    return LastCommand;
}

