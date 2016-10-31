//
// Created by Anthony on 5/16/2016.
//

#include "GameStateManager.hpp"

#include "Ranges.hpp"
#include "Engine.hpp"

namespace _detail_GameStateManager {

    void GameStateManager::push(std::function<StateErasure(std::string)> stateMaker)
    {
        std::string suid = PrintStr("%$", next_uid());
        stateModules.insert(make_pair(suid, std::vector<std::string>()));

        engine->ScriptEngine->BeginConfigGroup(suid.c_str());
        states.emplace_back(stateMaker(suid));
        engine->ScriptEngine->EndConfigGroup();

        if (states.back().haltsUpdate())
        {
            bool first = true;
            for (StateErasure& state : reverse(states))
            {
                if (first)
                {
                    first = false;
                    continue;
                }

                state.onPause();
                if (state.haltsUpdate())
                    break;
            }
        }
    }

    void GameStateManager::pop()
    {
        std::string suid = states.back().getSUID();

        for (auto moduleName : stateModules.at(suid))
        {
            if (engine->ConsoleModule->GetName() == moduleName)
                engine->ConsoleModule = engine->ScriptEngine->GetModule("ConsoleModule");
            engine->ScriptEngine->DiscardModule(moduleName.c_str());
        }
        stateModules.erase(suid);

        if (states.back().haltsUpdate())
        {
            bool first = true;
            for (StateErasure& state : reverse(states))
            {
                if (first)
                {
                    first = false;
                    continue;
                }

                state.onResume();
                if (state.haltsUpdate())
                    break;
            }
        }

        states.pop_back();

        int r;
        r = engine->ScriptEngine->GarbageCollect(asGC_FULL_CYCLE); assert(r >= 0);
        r = engine->ScriptEngine->RemoveConfigGroup(suid.c_str()); assert(r >= 0);
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
        for (StateErasure& state : reverse(states))
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
        for (StateErasure& state : reverse(states))
        {
            bool halts = state.haltsUpdate();
            auto c = states.size();
            state.update();
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
                auto halts = iter->haltsDraw();
                if (halts) return iter;
            }
            return begin(states);
        };

        auto start = findStart();
        for (auto& state : make_iterator_range(start, end(states)))
        {
            state.draw();
        }
    }

    void GameStateManager::registerModule(std::string suid, std::string moduleName)
    {
        stateModules.at(suid).emplace_back(moduleName);
    }
} // namespace _detail_GameStateManager
