//
// Created by Anthony on 5/3/2016.
//

#include "Terminal.hpp"
#include "Util.hpp"

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
    terminal->GetSignal(Terminal::OnCloseButton).Connect(std::bind([&]{ terminal->Show(false); }));

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

    terminal->CommandBufferMaxSize = 10;
    terminal->CommandBufferIndex = -1;

    terminal->Entry->GrabFocus();

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
                case sf::Keyboard::Key::Up:
                    AddToCommandBufferIndex(1);
                    break;
                case sf::Keyboard::Key::Down:
                    AddToCommandBufferIndex(-1);
                    break;
                case sf::Keyboard::Key::PageUp:
                    AddToCommandBufferIndex(CommandBufferMaxSize);
                    break;
                case sf::Keyboard::Key::PageDown:
                    AddToCommandBufferIndex(-CommandBufferMaxSize);
                    break;
            }
            break;
        case sf::Event::MouseWheelScrolled:
            if (IsMouseInWidget())
            {
                float d = event.mouseWheelScroll.delta;
                sfg::Adjustment::Ptr adj;

                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                    adj = ScrollWindow->GetVerticalAdjustment();
                else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
                    adj = ScrollWindow->GetHorizontalAdjustment();

                for (int i = 0; i < abs(d) * 14; i++)
                {
                    if (d < 0)
                        adj->Increment();
                    else
                        adj->Decrement();
                }
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

        CommandBuffer.push_front(text);
        if (CommandBuffer.size() > CommandBufferMaxSize)
            CommandBuffer.pop_back();

        GetSignals().Emit(OnCommandEntered);
    }
    CommandBufferIndex = -1;
    Entry->SetText("");
    Entry->GrabFocus();
}

std::string& Terminal::GetLastCommand()
{
    return CommandBuffer.front();
}

void Terminal::PrintClass(std::string Str, std::string Class)
{
    auto label = sfg::Label::Create(Str);
    label->SetAlignment(sf::Vector2f(0.f, 0.f));
    label->SetClass(Class);
    LogBox->PackEnd(label);

    auto adj = ScrollWindow->GetVerticalAdjustment();
    adj->SetValue(adj->GetUpper());
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

void Terminal::PrintWarning(std::string Str)
{
    PrintClass(Str, "Warning");
}

void Terminal::AddToCommandBufferIndex(int val)
{
    CommandBufferIndex += val;
    if (CommandBufferIndex < -1)
        CommandBufferIndex = -1;
    else if (CommandBufferIndex >= (int)CommandBuffer.size())
        CommandBufferIndex = CommandBuffer.size() - 1;

    if (CommandBufferIndex == -1)
        Entry->SetText("");
    else
        Entry->SetText(CommandBuffer.at(CommandBufferIndex));

    Entry->SetCursorPosition(Entry->GetText().getSize());
}

std::string Terminal::GetEntryText()
{
    return Entry->GetText();
}

void Terminal::SetEntryText(std::string str)
{
    Entry->SetText(str);
}
