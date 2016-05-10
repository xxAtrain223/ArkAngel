//
// Created by Anthony on 5/3/2016.
//

#include "Terminal.hpp"

#include <iostream>

using namespace std;

sfg::Signal::SignalID Terminal::OnCommandEntered = 0;

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
    windowBox->Pack(terminal->ScrollWindow, true, true);
    windowBox->Pack(entryBox, false, true);

    terminal->Add(windowBox);

    sfg::Context::Get().GetEngine().SetProperty("Terminal", "Gap", 10.f);
    sfg::Context::Get().GetEngine().SetProperty("Terminal", "BorderColor", sf::Color( 0x5a, 0x6a, 0x50 ));
    sfg::Context::Get().GetEngine().SetProperty("Terminal", "BorderColorShift", 0);
    sfg::Context::Get().GetEngine().SetProperty("Terminal", "TitleBackgroundColor", sf::Color( 0x5a, 0x6a, 0x50 ));
    sfg::Context::Get().GetEngine().SetProperty("Terminal", "TitlePadding", 5.f);
    sfg::Context::Get().GetEngine().SetProperty("Terminal", "HandleSize", 10.f);
    sfg::Context::Get().GetEngine().SetProperty("Terminal", "ShadowDistance", 3.f);
    sfg::Context::Get().GetEngine().SetProperty("Terminal", "ShadowAlpha", 100.f);
    sfg::Context::Get().GetEngine().SetProperty("Terminal", "CloseHeight", 10.f);
    sfg::Context::Get().GetEngine().SetProperty("Terminal", "CloseThickness", 3.f);

    terminal->RequestResize();
    return terminal;
}

const std::string& Terminal::GetName() const
{
    static const std::string name("Terminal");
    return name;
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
        Print("> " + text);

        LastCommand = text;
        GetSignals().Emit(OnCommandEntered);
    }
    Entry->SetText("");
    Entry->GrabFocus();
}

std::string& Terminal::GetLastCommand()
{
    return LastCommand;
}

void Terminal::PrintClass(std::string Str, std::string Class)
{
    auto label = sfg::Label::Create(Str);
    label->SetAlignment(sf::Vector2f(0.f, 0.f));
    label->SetClass(Class);
    LogBox->PackEnd(label);
}

void Terminal::Print(std::string Str)
{
    PrintClass(Str, "");
}

void Terminal::PrintLog(std::string Str)
{
    PrintClass(Str, "Log");
}

void Terminal::PrintError(std::string Str)
{
    PrintClass(Str, "Error");
}
