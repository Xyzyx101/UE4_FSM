#pragma once
#include <type_traits>
template< class, class = void >
struct IsSingleDelegate : std::false_type {};
template< class T >
struct IsSingleDelegate<T, std::enable_if_t<std::is_member_function_pointer<decltype(&T::Execute)>::value>> : std::true_type {};

template< class, class = void >
struct IsMulticastDelegate : std::false_type {};
template< class T >
struct IsMulticastDelegate<T, std::enable_if_t<std::is_member_function_pointer<decltype(&T::Broadcast)>::value>> : std::true_type {};

template<class StateEnum, class StateEvent>
class State
{
public:
	State(StateEnum enumType, StateEvent* enter, StateEvent* tick, StateEvent* exit) :
		Enum(enumType),
		EnterEvent(enter),
		TickEvent(tick),
		ExitEvent(exit) {
	}
	void Enter() const {
		if(EnterEvent) {
			if constexpr(IsSingleDelegate<StateEvent>::value) {
				EnterEvent->ExecuteIfBound();
			} else if constexpr(IsMulticastDelegate<StateEvent>::value) {
				EnterEvent->Broadcast();
			} else {
				static_assert(false, "Unknown delegate type");
			}
		}
	}
	void Tick() const {
		if(TickEvent) {
			if constexpr(IsSingleDelegate<StateEvent>::value) {
				TickEvent->ExecuteIfBound();
			} else if constexpr(IsMulticastDelegate<StateEvent>::value) {
				TickEvent->Broadcast();
			} else {
				static_assert(false, "Unknown delegate type");
			}
		}
	}
	void Exit() const {
		if(ExitEvent) {
			if constexpr(IsSingleDelegate<StateEvent>::value) {
				ExitEvent->ExecuteIfBound();
			} else if constexpr(IsMulticastDelegate<StateEvent>::value) {
				ExitEvent->Broadcast();
			} else {
				static_assert(false, "Unknown delegate type");
			}
		}
	}
	const StateEnum Enum;
protected:
	StateEvent* EnterEvent;
	StateEvent* TickEvent;
	StateEvent* ExitEvent;
};

//template<class StateEnum, class StateEvent>
//class StateBase {
//public:
//	StateBase(StateEnum enumType, StateEvent* enter, StateEvent* tick, StateEvent* exit) :
//		Enum(enumType),
//		EnterEvent(enter),
//		TickEvent(tick),
//		ExitEvent(exit) {
//	}
//	StateEnum Enum;
//
//protected:
//	StateEvent* EnterEvent;
//	StateEvent* TickEvent;
//	StateEvent* ExitEvent;
//};
//
//template<class StateEnum, class StateEvent, class DelegateType = void>
//class State {};
//template<class StateEnum, class StateEvent>
//class State<StateEnum, StateEvent, typename std::enable_if<IsSingleDelegate<StateEvent>::value>::type> : public StateBase<StateEnum, StateEvent>
//{
//public:
//	State(StateEnum enumType, StateEvent* enter, StateEvent* tick, StateEvent* exit) : StateBase(enumType, enter, tick, exit) {	}
//
//	void Enter() const {
//		if(EnterEvent) {
//			EnterEvent->ExecuteIfBound();
//		}
//	}
//	void Tick() const {
//		if(TickEvent) {
//			TickEvent->ExecuteIfBound();
//		}
//	}
//	void Exit() const {
//		if(ExitEvent) {
//			ExitEvent->ExecuteIfBound();
//		}
//	}
//};
//
//template<class StateEnum, class StateEvent>
//class State<StateEnum, StateEvent, typename std::enable_if<IsMulticastDelegate<StateEvent>::value>::type> : public StateBase<StateEnum, StateEvent>
//{
//public:
//	State(StateEnum enumType, StateEvent* enter, StateEvent* tick, StateEvent* exit) : StateBase(enumType, enter, tick, exit) {}
//
//	void Enter() {
//		if(EnterEvent) {
//			EnterEvent->Broadcast();
//		}
//	}
//	void Tick() {
//		if(TickEvent) {
//			TickEvent->Broadcast();
//		}
//	}
//	void Exit() {
//		if(ExitEvent) {
//			ExitEvent->Broadcast();
//		}
//	}
//};

