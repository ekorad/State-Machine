#pragma once

/* ************************************************************************** */
/* ******************************** INCLUDES ******************************** */
/* ************************************************************************** */

#include <tuple>
#include <variant>

/* ************************************************************************** */
/* ********************************** TYPES ********************************* */
/* ************************************************************************** */

namespace FSM
{
    template <typename... PossibleStates>
    class FiniteStateMachine
    {
    public:
        FiniteStateMachine() = default;
        FiniteStateMachine(PossibleStates&&... possibleStates);
        FiniteStateMachine(const PossibleStates&... possibleStates);

        template <typename TargetState> TargetState& performTransitionToState();

        template <typename Event> void handleEvent(const Event& event);

    private:
        std::tuple<PossibleStates...> _possibleStates;
        std::variant<PossibleStates*...> _currentState{ &std::get<0>(_possibleStates) };
    };
}

/* ************************************************************************** */
/* ***************************** IMPLEMENTATION ***************************** */
/* ************************************************************************** */

template <typename... PossibleStates>
FSM::FiniteStateMachine<PossibleStates...>::FiniteStateMachine
    (PossibleStates&&... possibleStates)
    : _possibleStates{ std::move(possibleStates)... } {}

template <typename... PossibleStates>
FSM::FiniteStateMachine<PossibleStates...>::FiniteStateMachine
    (const PossibleStates&... possibleStates)
    : _possibleStates{ possibleStates... } {}

template <typename... PossibleStates>
template <typename TargetState>
TargetState& FSM::FiniteStateMachine<PossibleStates...>::performTransitionToState()
{
    auto& targetState = std::get<TargetState>(_possibleStates);
    _currentState = &targetState;
    return targetState;
}

template <typename... PossibleStates>
template <typename Event>
void FSM::FiniteStateMachine<PossibleStates...>::handleEvent(const Event& event)
{
    auto passEventToState = [this, &event](auto statePtr)
    {
        auto action = statePtr->handleEvent(event);
        action.executeByStateMachine(*this, *statePtr, event);
    };
    std::visit(passEventToState, _currentState);
}