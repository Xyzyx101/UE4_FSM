#pragma once
#include "State.h"
#include "StateMachine.h"

template<typename StateEnum, typename StateEventType>
class Substate : public State<StateEnum, StateEventType>
{
public:
	Substate(StateEnum enumType, StateEventType* enter, StateEventType* tick, StateEventType* exit) :
		State(enumType, enter, tick, exit),
		Submachine(new StateMachine<StateEnum, StateEventType>{}) {
	}
	~Substate() = default;
	Substate(const Substate& other) :
		State(other) {
		this->Submachine = std::make_unique<StateMachine<StateEnum, StateEventType>>(*other.Submachine);
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
		Submachine->PendingState = Submachine->InitialState;
		if(EnterEvent) {
			static_assert(IsSingleDelegate<StateEventType>::value || IsMulticastDelegate<StateEventType>::value, "Unknown delegate type");
			if constexpr(IsSingleDelegate<StateEventType>::value) {
				EnterEvent->ExecuteIfBound();
			} else if constexpr(IsMulticastDelegate<StateEventType>::value) {
				EnterEvent->Broadcast();
			}
		}
	}
	void Tick() const override {
		if(TickEvent) {
			static_assert(IsSingleDelegate<StateEventType>::value || IsMulticastDelegate<StateEventType>::value, "Unknown delegate type");
			if constexpr(IsSingleDelegate<StateEventType>::value) {
				TickEvent->ExecuteIfBound();
			} else if constexpr(IsMulticastDelegate<StateEventType>::value) {
				TickEvent->Broadcast();
			}
		}
		Submachine->TickStateMachine(0);
	}
	void Exit() const override {
		Submachine->PendingState = Submachine->Unitialized;
		Submachine->TickStateMachine(0);
		Submachine->PendingState = static_cast<StateEnum>(0);
		if(ExitEvent) {
			static_assert(IsSingleDelegate<StateEventType>::value || IsMulticastDelegate<StateEventType>::value, "Unknown delegate type");
			if constexpr(IsSingleDelegate<StateEventType>::value) {
				ExitEvent->ExecuteIfBound();
			} else if constexpr(IsMulticastDelegate<StateEventType>::value) {
				ExitEvent->Broadcast();
			}
		}
	}
	std::unique_ptr<StateMachine<StateEnum, StateEventType>> Submachine;
};