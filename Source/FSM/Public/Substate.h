#pragma once
#include "State.h"
#include "StateMachine.h"

template<typename StateEnum, typename StateEventType, typename MachineStateEnum>
class Substate : public State<StateEnum, StateEventType>
{
public:
	Substate(StateEnum enumType, StateEventType* enter, StateEventType* tick, StateEventType* exit) :
		State(enumType, enter, tick, exit),
		Submachine(new StateMachine<MachineStateEnum, StateEventType>{}) {
	}
	~Substate() = default;
	Substate(const Substate& other) :
		State(other) {
		this.Submachine = std::make_unique<StateMachine<MachineStateEnum, StateEventType>>(*other.Submachine);
	};
	Substate& operator=(Substate other) {
		std::swap(*this, other);
		return *this;
	};
	Substate(Substate&& other) :
		State(other) {
		this->Submachine = std::move(other.Submachine);
	};
	Substate& operator=(Substate&& other) {
		State::operator=(std::move(other));
		this->Submachine = std::move(other.Submachine);
		return *this;
	}
	friend class StateMachine<StateEnum, StateEventType>;

protected:
	void Enter() const override {
		if(EnterEvent) {
			if constexpr(IsSingleDelegate<StateEventType>::value) {
				EnterEvent->ExecuteIfBound();
			} else if constexpr(IsMulticastDelegate<StateEventType>::value) {
				EnterEvent->Broadcast();
			} else {
				static_assert(false, "Unknown delegate type");
			}
		}
	}
	void Tick() const override {
		if(TickEvent) {
			if constexpr(IsSingleDelegate<StateEventType>::value) {
				TickEvent->ExecuteIfBound();
			} else if constexpr(IsMulticastDelegate<StateEventType>::value) {
				TickEvent->Broadcast();
			} else {
				static_assert(false, "Unknown delegate type");
			}
		}
		Submachine->TickStateMachine(0);
	}
	void Exit() const override {
		Submachine->PendingState = Submachine->Unitialized;
		Submachine->TickStateMachine(0);
		Submachine->PendingState = static_cast<MachineStateEnum>(0);
		if(ExitEvent) {
			if constexpr(IsSingleDelegate<StateEventType>::value) {
				ExitEvent->ExecuteIfBound();
			} else if constexpr(IsMulticastDelegate<StateEventType>::value) {
				ExitEvent->Broadcast();
			} else {
				static_assert(false, "Unknown delegate type");
			}
		}
	}
	std::unique_ptr<StateMachine<MachineStateEnum, StateEventType>> Submachine;
};