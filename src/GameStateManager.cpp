//
// Created by Anthony on 5/16/2016.
//

#include "GameStateManager.hpp"

#include "Ranges.hpp"
#include "Engine.hpp"

namespace _detail_GameStateManager {

    void GameStateManager::push(std::function<StateErasure()> state)
    {
        std::string uid = PrintStr("%$", next_uid());
        engine->ScriptEngine->BeginConfigGroup(uid.c_str());
        states.emplace_back(std::make_tuple(state(), uid));
        engine->ScriptEngine->EndConfigGroup();
    }

    void GameStateManager::pop()
    {
        engine->ScriptEngine->RemoveConfigGroup(std::get<1>(states.back()).c_str());
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
            //bool halts = state.haltsHandleEvent();
            bool halts = std::get<0>(state).haltsHandleEvent();
            std::get<0>(state).handleEvent(event);
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
            bool halts = std::get<0>(state).haltsUpdate();
            auto c = states.size();
            std::get<0>(state).update();
            if (c != states.size()) return;
            if (halts) return;
        }
    }

    void GameStateManager::draw()
    {
        auto findStart = [&]
        {
            for (auto riter = std::rbegin(states), eriter = std::rend(states); riter != eriter; ++riter)
            {
                auto iter = next(riter).base();
                auto halts = std::get<0>(*iter).haltsDraw();
                if (halts) return iter;
            }
            return begin(states);
        };

        auto start = findStart();
        for (auto& state : make_iterator_range(start, end(states)))
        {
            std::get<0>(state).draw();
        }
    }

} // namespace _detail_GameStateManager