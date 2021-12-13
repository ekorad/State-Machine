#pragma once

/* ************************************************************************** */
/* ********************************** TYPES ********************************* */
/* ************************************************************************** */

namespace FSM
{
    /* --------------------------- DefaultEventHandler -------------------------- */

    template <typename Action>
    class DefaultEventHandler
    {
    public:
        template <typename Event> Action handleEvent(const Event& event) const;
    };

    /* ------------------------------ EventHandler ------------------------------ */

    template <typename Event, typename Action>
    class EventHandler
    {
    public:
        Action handleEvent(const Event& event) const;
    };

    /* ---------------------------- EventHandlerGroup --------------------------- */

    template <typename... EventHandlers>
    class EventHandlerGroup : EventHandlers...
    {
    public:
        using EventHandlers::handleEvent...;
    };
}

/* ************************************************************************** */
/* ***************************** IMPLEMENTATION ***************************** */
/* ************************************************************************** */

/* --------------------------- DefaultEventHandler -------------------------- */

template <typename Action>
template <typename Event>
Action FSM::DefaultEventHandler<Action>::handleEvent(const Event& event) const
{
    return {};
}

/* ------------------------------ EventHandler ------------------------------ */

template <typename Event, typename Action>
Action FSM::EventHandler<Event, Action>::handleEvent(const Event& event) const
{
    return {};
}