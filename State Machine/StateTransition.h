#pragma once

/* ************************************************************************** */
/* ********************************** TYPES ********************************* */
/* ************************************************************************** */

namespace FSM
{
    /* ----------------------------- StateTransition ---------------------------- */

    template <typename TargetState>
    class StateTransition
    {
    public:
        template <typename StateMachine, typename State, typename Event>
        void executeByStateMachine(StateMachine& machine, State& previousState,
            const Event& event);

    private:
        void leave(...);

        template <typename State, typename Event>
        auto leave(State& state, const Event& event) -> decltype(state.onLeave(event));

        void enter(...);

        template <typename State, typename Event>
        auto enter(State& state, const Event& event) -> decltype(state.onEnter(event));
    };

    /* --------------------------- NullStateTransition -------------------------- */

    class NullStateTransition
    {
    public:
        template <typename StateMachine, typename State, typename Event>
        void executeByStateMachine(StateMachine& machine, State& previousState,
            const Event& event);
    };
}

/* ************************************************************************** */
/* ***************************** IMPLEMENTATION ***************************** */
/* ************************************************************************** */

/* ----------------------------- StateTransition ---------------------------- */

template <typename TargetState>
template <typename StateMachine, typename State, typename Event>
void FSM::StateTransition<TargetState>::executeByStateMachine
    (StateMachine& machine, State& previousState, const Event& event)
{
    leave(previousState);
    TargetState& targetState = machine.template performTransitionToState<TargetState>();
    enter(targetState, event);
}

template <typename TargetState>
void FSM::StateTransition<TargetState>::leave(...) {}

template <typename TargetState>
template <typename State, typename Event>
auto FSM::StateTransition<TargetState>::leave(State& state, const Event& event)
    -> decltype(state.onLeave(event))
{
    return state.onLeave(event);
}

template <typename TargetState>
void FSM::StateTransition<TargetState>::enter(...) {}

template <typename TargetState>
template <typename State, typename Event>
auto FSM::StateTransition<TargetState>::enter(State& state, const Event& event)
    -> decltype(state.onEnter(event))
{
    return state.onEnter(event);
}

/* --------------------------- NullStateTransition -------------------------- */

template <typename StateMachine, typename State, typename Event>
void FSM::NullStateTransition::executeByStateMachine(StateMachine& machine,
    State& previousState, const Event& event) {}
