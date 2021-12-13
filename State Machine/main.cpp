#include <iostream>

#include "FiniteStateMachine.h"
#include "StateTransition.h"
#include "EventHandler.h"
#include "ActionExecutor.h"

using namespace FSM;

class OpenEvent {};
class CloseEvent {};
class LockEvent
{
public:
    uint32_t _newKey;
};
class UnlockEvent
{
public:
    uint32_t _key;
};

class OpenState;
class ClosedState;
class LockedState;

class OpenState : public EventHandlerGroup<
        DefaultEventHandler<NullStateTransition>,
        EventHandler<CloseEvent, StateTransition<ClosedState>>
    > {};

class ClosedState : public EventHandlerGroup<
        DefaultEventHandler<NullStateTransition>,
        EventHandler<OpenEvent, StateTransition<OpenState>>,
        EventHandler<LockEvent, StateTransition<LockedState>>
    > {};

class LockedState : public DefaultEventHandler<NullStateTransition>
{
public:
    using DefaultEventHandler::handleEvent;

    LockedState(uint32_t key)
        : _key(key) {}

    OptionalActionExecutor<StateTransition<ClosedState>> handleEvent(const UnlockEvent& event) const
    {
        if (event._key == _key) {
            return StateTransition<ClosedState>{};
        }
        return NullStateTransition{};
    }

    void onEnter(const LockEvent& event)
    {
        _key = event._newKey;
    }

private:
    uint32_t _key;
};

int main()
{
    FiniteStateMachine fsm{ ClosedState{}, OpenState{}, LockedState{ 12345 } };
    fsm.handleEvent(LockEvent{ 666 });
    fsm.handleEvent(UnlockEvent{ 12345 });
    fsm.handleEvent(OpenEvent{});
    return 0;
}