#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "FSM.h"
#include "State.h"

template<typename MachineStateEnum, typename StateEventType>
class StateMachine
{
protected:
	using StateType = State<MachineStateEnum, StateEventType>;
	using StatePtr = std::unique_ptr<StateType>;
	static const MachineStateEnum Unitialized = static_cast<MachineStateEnum>(~0);
	/*
	Note about the initial state.  States[0] is a hard coded 'initial' state that does nothing.
	PendingState is the 0 value of the state enum. The first tick will enter whatever state is assigned
	to 0.  If you want a different first state just call ChangeState at intialization time.
	Calling	ChangeState in the Enter of a substate can also be used to reset submachines
	*/
	StateMachine() :
		PendingState(Unitialized),
		InitialState(Unitialized),
		StateIdx(0) {
		States.emplace_back(std::make_unique<StateType>(Unitialized, nullptr, nullptr, nullptr));
	}
public:
	~StateMachine() = default;
	StateMachine(const StateMachine& other) :
		PendingState(other.PendingState),
		InitialState(other.InitialState),
		StateIdx(other.StateIdx) {
		this->States.reserve(other.States.size());
		for(auto& s : other.States) {
			this->States.emplace_back(std::unique_ptr<IStateBase<MachineStateEnum, StateEventType>>(s->Clone()));
		}
	};
	StateMachine& operator=(const StateMachine& other) {
		return *this = StateMachine(other);
	};
	StateMachine(StateMachine&& other) = default;
	StateMachine& operator=(StateMachine&& other) = default;

	template<typename, typename> friend class StateMachine;
	template<typename, typename> friend class Substate;

protected:
	// Initialization requires adding states and setting the initial state
	void AddState(MachineStateEnum state, StateEventType* enter, StateEventType* tick, StateEventType* exit) {
		checkf(state != Unitialized, TEXT("Reserved for unitialized state"));
		States.emplace_back(std::make_unique<StateType>(state, enter, tick, exit));
		if(InitialState == Unitialized) {
			InitialState = state;
			PendingState = InitialState;
		}
	}

	// Adds a state that is itself a state machine
	StateMachine<MachineStateEnum, StateEventType>& AddSubmachineState(MachineStateEnum state, StateEventType* enter, StateEventType* tick, StateEventType* exit) {
		checkf(state != Unitialized, TEXT("Reserved for unitialized state"));
		using SubstateType = Substate<MachineStateEnum, StateEventType>;
		std::unique_ptr<SubstateType> subStatePtr = std::make_unique<SubstateType>(state, enter, tick, exit);
		auto& machine = *(subStatePtr->Submachine);
		States.push_back(std::move(subStatePtr));
		return machine;
	}

	// Adds a state to a SubmachineState
	void AddSubstate(StateMachine<MachineStateEnum, StateEventType>& machine, MachineStateEnum state, StateEventType* enter, StateEventType* tick, StateEventType* exit) {
		machine.AddState(state, enter, tick, exit);
	}

	/*
	If the machine is running calling change state multiple time with the same state does nothing.
	Calling change state multiple times with different states in the same frame is safe and only does the last change.
	Exit (if it exists) on the current state will fire once and Enter (if it exists) will fire once both on the frame after
	ChangeState was called.  Tick will fire once and only once each frame on the current state this frame and on the
	new state next frame.
	*/
	void ChangeState(MachineStateEnum newState) {
		PendingState = newState;
	}

	MachineStateEnum GetState() const { return States[StateIdx]->GetState(); }

	void SetInitialState(MachineStateEnum state) {
		if(PendingState == InitialState) {
			PendingState = state;
		}
		InitialState = state;
	}

	// The owning class must tick the state machine
	void TickStateMachine(float dt) {
		const auto& current = States[StateIdx];
		if(current->GetState() == PendingState) {
			current->Tick();
		} else {
			auto pending = std::find_if(States.begin(), States.end(), [needle = PendingState](const auto& state) {return state->GetState() == needle; });
			if(pending == States.end()) {
				UE_LOG(LogFSM, Error, TEXT("Attempting to change to unknown state!!!"));
				return;
			}
			StateIdx = std::distance(States.begin(), pending);
			current->Exit();
			(*pending)->Enter();
			(*pending)->Tick();
		}
	}

protected:
	MachineStateEnum PendingState;
	MachineStateEnum InitialState;
	std::vector<std::unique_ptr<IStateBase<MachineStateEnum, StateEventType>>> States;
	SIZE_T StateIdx;
};

#include "Substate.h"
