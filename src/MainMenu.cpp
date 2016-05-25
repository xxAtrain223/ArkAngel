//
// Created by Anthony on 5/16/2016.
//

#include "MainMenu.hpp"
#include "ScriptSandbox.hpp"

#include <iostream>
#include <cstdio>

using namespace std;

MainMenu::MainMenu(Engine *engine) : engine(engine) {
    if (!optionFont.loadFromFile("data/fonts/OpenSans-Regular.ttf"))
        throw "Couldn't find 'data/fonts/OpenSans-Regular.ttf'";

    items.emplace_back(make_pair("Script Sandbox", StateMaker<ScriptSandbox>(engine)));
}

void MainMenu::update() {
    if (engine->wasKeyPressed(sf::Keyboard::Escape))
    {
        engine->Gsm.pop();
        return;
    }

    if (!items.empty()) {
        if (engine->wasKeyPressed(sf::Keyboard::Up) || engine->wasKeyPressed(sf::Keyboard::W))
            selected = (selected + int(items.size()) - 1) % int(items.size());

        if (engine->wasKeyPressed(sf::Keyboard::Down) || engine->wasKeyPressed(sf::Keyboard::S))
            selected = (selected + int(items.size()) + 1) % int(items.size());

        if (engine->wasKeyPressed(sf::Keyboard::Return) || engine->wasKeyPressed(sf::Keyboard::Space))
        {
            engine->Gsm.push(items[selected].second);
            return;
        }

        if (engine->wasMouseButtonPressed(sf::Mouse::Left))
        {
            sf::Text text;
            text.setFont(optionFont);
            text.setCharacterSize(40);

            for (int i = 0; i < items.size(); i++)
            {
                text.setString(items[i].first);
                text.setOrigin(text.getLocalBounds().width / 2.f, 0);
                text.setPosition(engine->Window.getSize().x / 2.f,
                                 100 + text.getFont()->getLineSpacing(text.getCharacterSize()) * (i + 1));
                sf::FloatRect boundingBox;
                boundingBox = text.getGlobalBounds();
                if (boundingBox.contains(engine->getMousePosition().x, engine->getMousePosition().y))
                {
                    selected = i;
                    engine->Gsm.push(items[selected].second);
                    return;
                }
            }
        }
    }
}

void MainMenu::draw() {
    engine->Window.clear(sf::Color(0, 127, 255));
    sf::Text optionText;
    optionText.setFont(optionFont);
    optionText.setCharacterSize(40);
    optionText.setColor(sf::Color::White);

    optionText.setString(engine->GetWindowTitle());
    optionText.setOrigin(sf::Vector2f(optionText.getLocalBounds().width / 2, 0));
    optionText.setPosition(engine->Window.getSize().x / 2.f, 100);

    engine->Window.draw(optionText);

    for (int i = 0; i < items.size(); i++) {
        optionText.setString(items[i].first);
        optionText.setOrigin(sf::Vector2f(optionText.getLocalBounds().width / 2, 0));
        optionText.setPosition(engine->Window.getSize().x / 2.f, 100 + optionText.getFont()->getLineSpacing(optionText.getCharacterSize()) * (i + 1));

        if (i == selected)
            optionText.setColor(sf::Color::Yellow);
        else
            optionText.setColor(sf::Color::White);

        engine->Window.draw(optionText);
    }
}
