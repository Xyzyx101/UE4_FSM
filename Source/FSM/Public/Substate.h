#pragma once

#include "State.h"
#include "StateMachine.h"

template<typename StateEnum, typename StateEvent>
class Substate : State<StateEnum, StateEvent>, StateMachine<StateEnum, StateEvent>
{

};
