#pragma once

/* ************************************************************************** */
/* ******************************** INCLUDES ******************************** */
/* ************************************************************************** */

#include <variant>

/* ************************************************************************** */
/* ********************************** TYPES ********************************* */
/* ************************************************************************** */

namespace FSM
{
    /* ----------------------------- ActionExecutor ----------------------------- */

    template <typename... PossibleActions>
    class ActionExecutor
    {
    public:
        template <typename Action> ActionExecutor(Action&& action);

        template <typename StateMachine, typename State, typename Event>
        void executeByStateMachine(StateMachine& machine, State& prevState,
            const Event& event);

    private:
        std::variant<PossibleActions...> _possibleActions;
    };

    /* ------------------------- OptionalActionExecutor ------------------------- */

    template <typename Action>
    class OptionalActionExecutor : public ActionExecutor<Action, NullStateTransition>
    {
    public:
        using ActionExecutor<Action, NullStateTransition>::ActionExecutor;
    };
}

/* ************************************************************************** */
/* ***************************** IMPLEMENTATION ***************************** */
/* ************************************************************************** */

/* ----------------------------- ActionExecutor ----------------------------- */

template <typename... PossibleActions>
template <typename Action>
FSM::ActionExecutor<PossibleActions...>::ActionExecutor(Action&& action)
    : _possibleActions{ std::forward<Action>(action) } {}

template <typename... PossibleActions>
template <typename StateMachine, typename State, typename Event>
void FSM::ActionExecutor<PossibleActions...>::executeByStateMachine
(StateMachine& machine, State& prevState,
    const Event& event)
{
    auto actionExecutor = [&machine, &prevState, &event](auto& action)
    {
        action.executeByStateMachine(machine, prevState, event);
    };
    std::visit(actionExecutor, _possibleActions);
}