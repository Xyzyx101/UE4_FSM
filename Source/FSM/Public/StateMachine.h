#pragma once

#include "State.h"
//#include "Substate.h"
#include <vector>
#include <algorithm>

template<typename StateEnum, typename StateEvent>
class StateMachine
{
protected:
	using StateType = State<StateEnum, StateEvent>;
	StateMachine() :
		StateIdx(0) {
		States.push_back(StateType(static_cast<StateEnum>(~0), nullptr, nullptr, nullptr));
	}

	// Initialization requires adding states and setting the initial state
	void AddState(StateEnum state, StateEvent* enter, StateEvent* tick, StateEvent* exit) {
		checkf(state != static_cast<StateEnum>(~0), TEXT("~0 is reserved for initial state"));
		States.push_back(StateType(state, enter, tick, exit));
	}

	/*
	If the machine is running calling change state multiple time with the same state does nothing.
	Calling change state multiple times with different states in the same frame is safe and only does the last change.
	Exit (if it exists) on the current state will fire once and Enter (if it exists) will fire once both on the frame after
	ChangeState was called.  Tick will fire once and only once each frame on the current state this frame and on the
	new state next frame.
	*/
	void ChangeState(StateEnum newState) {
		PendingState = newState;
	}

	StateEnum GetState() const { return States[StateIdx].Enum; }

	// The owning class must tick the state machine
	void TickStateMachine(float dt) {
		const auto& current = States[StateIdx];
		if(current.Enum == PendingState) {
			current.Tick();
		} else {
			auto pending = std::find_if(States.begin(), States.end(), [needle = PendingState](const StateType& state) {return state.Enum == needle; });
			if(pending == States.end()) {
				UE_LOG(LogFSM, Error, TEXT("Attempting to change to unknown state!!!"));
				return;
			}
			StateIdx = std::distance(States.begin(), pending);
			current.Exit();
			pending->Enter();
			pending->Tick();
		}
	}

	// Initialization requires adding states and setting the initial state
	/*State<StateEnum, StateEvent>& AddSubstate(StateEnum state, StateEvent* enter, StateEvent* tick, StateEvent* exit) {
		checkf(state != static_cast<StateEnum>(~0), TEXT("~0 is reserved for initial state"));
		States.emplace(State<StateEnum, StateEvent>(state, enter, tick, exit));
		return States.back();
	}*/

protected:
	StateEnum PendingState;
	std::vector<StateType> States;
	SIZE_T StateIdx;
};

