//
// Created by Anthony on 5/16/2016.
//

#include "GameStateManager.hpp"

#include "Ranges.hpp"
#include "Engine.hpp"

namespace _detail_GameStateManager {

    void GameStateManager::push(StateErasure state)
    {
        states.emplace_back(std::move(state));
    }

    void GameStateManager::pop()
    {
        states.pop_back();
    }

    bool GameStateManager::empty()
    {
        return states.empty();
    }

    int GameStateManager::size()
    {
        return states.size();
    }

    void GameStateManager::handleEvent(sf::Event event)
    {
        for (auto& state : reverse(states))
        {
            bool halts = state.haltsHandleEvent();
            state.handleEvent(event);
            if (halts)
            {
                return;
            }
        }
    }

    void GameStateManager::update()
    {
        for (auto& state : reverse(states))
        {
            bool halts = state.haltsUpdate();
            state.update();
            if (halts)
            {
                return;
            }
        }
    }

    void GameStateManager::draw()
    {
        auto findStart = [&]
        {
            for (auto riter = std::rbegin(states), eriter = std::rend(states); riter != eriter; ++riter)
            {
                auto iter = next(riter).base();
                auto halts = iter->haltsDraw();
                if (halts)
                {
                    return iter;
                }
            }
            return begin(states);
        };

        auto start = findStart();
        for (auto& state : make_iterator_range(start, end(states)))
        {
            state.draw();
        }
    }

} // namespace _detail_GameStateManager