#pragma once
#include <type_traits>
#include <memory>

template< class, class = void >
struct IsSingleDelegate : std::false_type {};
template< class T >
struct IsSingleDelegate<T, std::enable_if_t<std::is_member_function_pointer<decltype(&T::Execute)>::value>> : std::true_type {};

template< class, class = void >
struct IsMulticastDelegate : std::false_type {};
template< class T >
struct IsMulticastDelegate<T, std::enable_if_t<std::is_member_function_pointer<decltype(&T::Broadcast)>::value>> : std::true_type {};

template<typename StateEnum, typename StateEventType>
class IStateBase
{
public:
	virtual ~IStateBase() = default;
	virtual StateEnum GetState() const = 0;
	virtual void Enter() const = 0;
	virtual void Tick() const = 0;
	virtual void Exit() const = 0;
	virtual IStateBase* Clone() const = 0;
protected:
	/*IStateBase() {};
	IStateBase(const IStateBase& other) = default;
	IStateBase(IStateBase&& other) = default;*/
};

template<typename StateEnum, typename StateEventType>
class State : public IStateBase<StateEnum, StateEventType>
{
public:
	State(StateEnum stateTag, StateEventType* enter, StateEventType* tick, StateEventType* exit) :
		StateTag(stateTag),
		EnterEvent(enter),
		TickEvent(tick),
		ExitEvent(exit) {
	}
	~State() = default;
	State(const State& other) = default;
	State& operator=(const State& other) = default;
	State(State&& other) = default;
	State& operator=(State&& other) = default;
	virtual State* Clone() const {
		return new State(*this);
	}
	StateEnum GetState() const { return StateTag; }
	void Enter() const override {
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
	}
	void Exit() const override {
		if(ExitEvent) {
			static_assert(IsSingleDelegate<StateEventType>::value || IsMulticastDelegate<StateEventType>::value, "Unknown delegate type");
			if constexpr(IsSingleDelegate<StateEventType>::value) {
				ExitEvent->ExecuteIfBound();
			} else if constexpr(IsMulticastDelegate<StateEventType>::value) {
				ExitEvent->Broadcast();
			}
		}
	}
protected:
	const StateEnum StateTag;
	StateEventType* EnterEvent;
	StateEventType* TickEvent;
	StateEventType* ExitEvent;
};
