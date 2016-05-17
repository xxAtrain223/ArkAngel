//
// Created by Anthony on 5/16/2016.
//

#ifndef ARKANGEL_GAMESTATEMANAGER_HPP
#define ARKANGEL_GAMESTATEMANAGER_HPP

#include "Ranges.hpp"

#include <raspberry/raspberry.hpp>
#include <sfml/Window/Event.hpp>

#include <utility>
#include <vector>
#include <memory>

// Reflection stuff
class Engine;
namespace _detail_GameStateManager {

// Concepts
    RASPBERRY_DECL_METHOD(has_haltsHandleEvent, haltsHandleEvent);
    RASPBERRY_DECL_METHOD(has_haltsUpdate, haltsUpdate);
    RASPBERRY_DECL_METHOD(has_haltsDraw, haltsDraw);
    RASPBERRY_DECL_METHOD(has_handleEvent, handleEvent);
    RASPBERRY_DECL_METHOD(has_update, update);
    RASPBERRY_DECL_METHOD(has_draw, draw);

// Erasure Type

    using StateErasure = raspberry::Any<
            has_haltsHandleEvent<bool()>,
            has_haltsUpdate<bool()>,
            has_haltsDraw<bool()>,
            has_handleEvent<void(sf::Event)>,
            has_update<void()>,
            has_draw<void()>
    >;

// State manager

/**
 * Manages a stack of game states
 *
 * Manages and provides access to a stack of game states.
 * A game state can be anything.
 *
 * Absolutely not thread-safe.
 */
    class GameStateManager final
    {
        Engine* engine;

        std::vector<StateErasure> states;

    public:
        GameStateManager(Engine* engine) : engine(engine) {}

        /**
         * Pushes a state onto the stack
         *
         * Pushes a state onto the top of the stack.
         * This state will be updated first and drawn last.
         *
         * This function must not be called while update() or draw() is running.
         *
         * @param state Game state
         */
        void push(StateErasure state);

        /**
         * Pops a state off the stack
         *
         * Pops the current top state off the stack.
         *
         * This function must not be called while update() or draw() is running.
         */
        void pop();

        void handleEvent(sf::Event event);

        /**
         * Update states
         *
         * Updates all states on the stack, from top to bottom.
         * This is done by calling `state.update()` or `update(state)`,
         * if available.
         *
         * A state may provide `state.haltsUpdate()` or `haltsUpdate(state)`.
         * If that function returns `true`, this state will prevent states
         * below it from being updated.
         */
        void update();

        /**
         * Draw states
         *
         * Draws all states on the stack, from bottom to top.
         * This is done by calling `state.draw()` or `draw(state)`,
         * if available.
         *
         * A state may provide `state.haltsDraw()` or `haltsDraw(state)`.
         * If that function returns `true`, this state will prevent states
         * below it from being drawn.
         */
        void draw();

        bool empty();

        int size();
    };

} // namespace _detail_GameStateManager

using _detail_GameStateManager::GameStateManager;
using _detail_GameStateManager::StateErasure;

#endif //ARKANGEL_GAMESTATEMANAGER_HPP
