#include "TestActors.h"
#include "FSM.h"

/*
ATestActorBase
*/

ATestActorBase::ATestActorBase() :
	Logger(nullptr) {
	PrimaryActorTick.bCanEverTick = true;
}

void ATestActorBase::Tick(float DeltaTime) {
	TickStateMachine(DeltaTime);
	Super::Tick(DeltaTime);
}

FStateTestLogger<FString>* ATestActorBase::RegisterTestRunner(FAutomationTestBase* testRunner) {
	Logger = MakeUnique<FStateTestLogger<FString>>(testRunner);
	return Logger.Get();;
}


/*
ATestEnter
*/

ATestEnter::ATestEnter() {
	State1EnterDelegate.BindUObject(this, &ATestEnter::State1_Enter);
	AddState(EActorTestState::State1, &State1EnterDelegate, nullptr, nullptr);
	State2EnterDelegate.BindUObject(this, &ATestEnter::State2_Enter);
	AddState(EActorTestState::State2, &State2EnterDelegate, nullptr, nullptr);
	ChangeState(EActorTestState::State1);
}

void ATestEnter::State1_Enter() {
	UE_LOG(LogFSM, Log, TEXT("State1_Enter"));
	if(Logger) {
		Logger->Log(TEXT("State1_Enter"));
	}
}

void ATestEnter::State2_Enter() {
	UE_LOG(LogFSM, Log, TEXT("State2_Enter"));
	if(Logger) {
		Logger->Log(TEXT("State2_Enter"));
	}
}


/*
ATestEnterTickExit
*/

ATestEnterTickExit::ATestEnterTickExit() {
	State1EnterDelegate.BindUObject(this, &ATestEnterTickExit::State1_Enter);
	State1TickDelegate.BindUObject(this, &ATestEnterTickExit::State1_Tick);
	State1ExitDelegate.BindUObject(this, &ATestEnterTickExit::State1_Exit);
	AddState(EActorTestState::State1, &State1EnterDelegate, &State1TickDelegate, &State1ExitDelegate);
	State2EnterDelegate.BindUObject(this, &ATestEnterTickExit::State2_Enter);
	State2TickDelegate.BindUObject(this, &ATestEnterTickExit::State2_Tick);
	State2ExitDelegate.BindUObject(this, &ATestEnterTickExit::State2_Exit);
	AddState(EActorTestState::State2, &State2EnterDelegate, &State2TickDelegate, &State2ExitDelegate);
	ChangeState(EActorTestState::State1);
}

void ATestEnterTickExit::State1_Enter() {
	UE_LOG(LogFSM, Log, TEXT("State1_Enter"));
	if(Logger) {
		Logger->Log(TEXT("State1_Enter"));
	}
}

void ATestEnterTickExit::State1_Tick() {
	UE_LOG(LogFSM, Log, TEXT("State1_Tick"));
	if(Logger) {
		Logger->Log(TEXT("State1_Tick"));
	}
}

void ATestEnterTickExit::State1_Exit() {
	UE_LOG(LogFSM, Log, TEXT("State1_Exit"));
	if(Logger) {
		Logger->Log(TEXT("State1_Exit"));
	}
}

void ATestEnterTickExit::State2_Enter() {
	UE_LOG(LogFSM, Log, TEXT("State2_Enter"));
	if(Logger) {
		Logger->Log(TEXT("State2_Enter"));
	}
}

void ATestEnterTickExit::State2_Tick() {
	UE_LOG(LogFSM, Log, TEXT("State2_Tick"));
	if(Logger) {
		Logger->Log(TEXT("State2_Tick"));
	}
}

void ATestEnterTickExit::State2_Exit() {
	UE_LOG(LogFSM, Log, TEXT("State2_Exit"));
	if(Logger) {
		Logger->Log(TEXT("State2_Exit"));
	}
}


/*
ATestSequence
*/

ATestSequence::ATestSequence() {
	State1EnterDelegate.BindUObject(this, &ATestSequence::State1_Enter);
	State1TickDelegate.BindUObject(this, &ATestSequence::State1_Tick);
	AddState(EActorTestState::State1, &State1EnterDelegate, &State1TickDelegate, nullptr);
	State2EnterDelegate.BindUObject(this, &ATestSequence::State2_Enter);
	AddState(EActorTestState::State2, &State2EnterDelegate, nullptr, nullptr);
	State3EnterDelegate.BindUObject(this, &ATestSequence::State3_Enter);
	AddState(EActorTestState::State3, &State3EnterDelegate, nullptr, nullptr);
	ChangeState(EActorTestState::State1);
}

void ATestSequence::State1_Enter() {
	UE_LOG(LogFSM, Log, TEXT("State1_Enter"));
	if(Logger) {
		Logger->Log(TEXT("State1_Enter"));
	}
}

void ATestSequence::State1_Tick() {
	UE_LOG(LogFSM, Log, TEXT("State1_Tick"));
	if(Logger) {
		Logger->Log(TEXT("State1_Tick"));
	}
}

void ATestSequence::State2_Enter() {
	UE_LOG(LogFSM, Log, TEXT("State2_Enter"));
	if(Logger) {
		Logger->Log(TEXT("State2_Enter"));
	}
	ChangeState(EActorTestState::State3);
}

void ATestSequence::State3_Enter() {
	UE_LOG(LogFSM, Log, TEXT("State3_Enter"));
	if(Logger) {
		Logger->Log(TEXT("State3_Enter"));
	}
	ChangeState(EActorTestState::State1);
}


/*
ATestPause
*/

ATestPause::ATestPause() {
	RunTickDelegate.BindUObject(this, &ATestPause::Run_Tick);
	AddState(EActorTestState::Run, nullptr, &RunTickDelegate, nullptr);
	PauseEnterDelegate.BindUObject(this, &ATestPause::Pause_Enter);
	PauseExitDelegate.BindUObject(this, &ATestPause::Pause_Exit);
	AddState(EActorTestState::Pause, &PauseEnterDelegate, nullptr, &PauseExitDelegate);
	ChangeState(EActorTestState::Run);
}

void ATestPause::Run_Tick() {
	UE_LOG(LogFSM, Log, TEXT("Run_Tick"));
	if(Logger) {
		Logger->Log(TEXT("Run_Tick"));
	}
}

void ATestPause::Pause_Enter() {
	UE_LOG(LogFSM, Log, TEXT("Pause_Enter"));
	if(Logger) {
		Logger->Log(TEXT("Pause_Enter"));
	}
}

void ATestPause::Pause_Exit() {
	UE_LOG(LogFSM, Log, TEXT("Pause_Exit"));
	if(Logger) {
		Logger->Log(TEXT("Pause_Exit"));
	}
}


/*
APingPong
*/

APingPong::APingPong() {
	PingEvent.BindUObject(this, &APingPong::Ping);
	AddState(EActorTestState::Ping, nullptr, &PingEvent, nullptr);
	PongEvent.BindUObject(this, &APingPong::Pong);
	AddState(EActorTestState::Pong, nullptr, &PongEvent, nullptr);
	ChangeState(EActorTestState::Ping);
}

void APingPong::Ping() {
	UE_LOG(LogFSM, Log, TEXT("Ping State"));
	if(Logger) {
		Logger->Log(TEXT("Ping"));
	}
	ChangeState(EActorTestState::Pong);
}

void APingPong::Pong() {
	UE_LOG(LogFSM, Log, TEXT("Pong State"));
	if(Logger) {
		Logger->Log(TEXT("Pong"));
	}
	ChangeState(EActorTestState::Ping);
}


/*
ATestEnterSubstate
*/

ATestEnterSubstate::ATestEnterSubstate() {
	State1EnterDelegate.BindUObject(this, &ATestEnterSubstate::State1_Enter);
	StateMachine<EActorTestSubstateA, FTestSignature>& state1Machine = AddSubmachineState<EActorTestSubstateA>(EActorTestState::State1, &State1EnterDelegate, nullptr, nullptr);
	Substate1EnterDelegate.BindUObject(this, &ATestEnterSubstate::Substate1_Enter);
	AddSubstate<EActorTestSubstateA, FTestSignature>(state1Machine, EActorTestSubstateA::Substate_A1, &Substate1EnterDelegate, nullptr, nullptr);

	State2EnterDelegate.BindUObject(this, &ATestEnterSubstate::State2_Enter);
	StateMachine<EActorTestSubstateB, FTestSignature>& state2Machine = AddSubmachineState<EActorTestSubstateB>(EActorTestState::State2, &State2EnterDelegate, nullptr, nullptr);
	Substate2EnterDelegate.BindUObject(this, &ATestEnterSubstate::Substate2_Enter);
	AddSubstate<EActorTestSubstateB, FTestSignature>(state2Machine, EActorTestSubstateB::Substate_B1, &Substate2EnterDelegate, nullptr, nullptr);

	ChangeState(EActorTestState::State1);
}

void ATestEnterSubstate::State1_Enter() {
	UE_LOG(LogFSM, Log, TEXT("State1_Enter"));
	if(Logger) {
		Logger->Log(TEXT("State1_Enter"));
	}
}

void ATestEnterSubstate::State2_Enter() {
	UE_LOG(LogFSM, Log, TEXT("State2_Enter"));
	if(Logger) {
		Logger->Log(TEXT("State2_Enter"));
	}
}

void ATestEnterSubstate::Substate1_Enter() {
	UE_LOG(LogFSM, Log, TEXT("Substate_A1_Enter"));
	if(Logger) {
		Logger->Log(TEXT("Substate_A1_Enter"));
	}
}

void ATestEnterSubstate::Substate2_Enter() {
	UE_LOG(LogFSM, Log, TEXT("Substate_B1_Enter"));
	if(Logger) {
		Logger->Log(TEXT("Substate_B1_Enter"));
	}
}


/*
ATestFullSubstate
*/
ATestFullSubstate::ATestFullSubstate() {
	State1EnterDelegate.BindUObject(this, &ATestFullSubstate::State1_Enter);
	State1TickDelegate.BindUObject(this, &ATestFullSubstate::State1_Tick);
	State1ExitDelegate.BindUObject(this, &ATestFullSubstate::State1_Exit);
	State2EnterDelegate.BindUObject(this, &ATestFullSubstate::State2_Enter);
	State2TickDelegate.BindUObject(this, &ATestFullSubstate::State2_Tick);
	State2ExitDelegate.BindUObject(this, &ATestFullSubstate::State2_Exit);
	SubstateAEnterDelegate.BindUObject(this, &ATestFullSubstate::SubstateA_Enter);
	SubstateATickDelegate.BindUObject(this, &ATestFullSubstate::SubstateA_Tick);
	SubstateAExitDelegate.BindUObject(this, &ATestFullSubstate::SubstateA_Exit);
	SubstateBEnterDelegate.BindUObject(this, &ATestFullSubstate::SubstateB_Enter);
	SubstateBTickDelegate.BindUObject(this, &ATestFullSubstate::SubstateB_Tick);
	SubstateBExitDelegate.BindUObject(this, &ATestFullSubstate::SubstateB_Exit);

	StateMachine<EActorTestSubstateA, FTestSignature>& state1Machine = AddSubmachineState<EActorTestSubstateA>(EActorTestState::State1, &State1EnterDelegate, &State1TickDelegate, &State1ExitDelegate);
	AddSubstate<EActorTestSubstateA, FTestSignature>(state1Machine, EActorTestSubstateA::Substate_A1, &SubstateAEnterDelegate, &SubstateATickDelegate, &SubstateAExitDelegate);
	StateMachine<EActorTestSubstateB, FTestSignature>& state2Machine = AddSubmachineState<EActorTestSubstateB>(EActorTestState::State2, &State2EnterDelegate, &State2TickDelegate, &State2ExitDelegate);
	AddSubstate<EActorTestSubstateB, FTestSignature>(state2Machine, EActorTestSubstateB::Substate_B1, &SubstateBEnterDelegate, &SubstateBTickDelegate, &SubstateBExitDelegate);
	ChangeState(EActorTestState::State1);
}

void ATestFullSubstate::State1_Enter() {
	UE_LOG(LogFSM, Log, TEXT("State1_Enter"));
	if(Logger) {
		Logger->Log(TEXT("State1_Enter"));
	}
}
void ATestFullSubstate::State1_Tick() {
	UE_LOG(LogFSM, Log, TEXT("State1_Tick"));
	if(Logger) {
		Logger->Log(TEXT("State1_Tick"));
	}
}
void ATestFullSubstate::State1_Exit() {
	UE_LOG(LogFSM, Log, TEXT("State1_Exit"));
	if(Logger) {
		Logger->Log(TEXT("State1_Exit"));
	}
}
void ATestFullSubstate::State2_Enter() {
	UE_LOG(LogFSM, Log, TEXT("State2_Enter"));
	if(Logger) {
		Logger->Log(TEXT("State2_Enter"));
	}
}
void ATestFullSubstate::State2_Tick() {
	UE_LOG(LogFSM, Log, TEXT("State2_Tick"));
	if(Logger) {
		Logger->Log(TEXT("State2_Tick"));
	}
}
void ATestFullSubstate::State2_Exit() {
	UE_LOG(LogFSM, Log, TEXT("State2_Exit"));
	if(Logger) {
		Logger->Log(TEXT("State2_Exit"));
	}
}
void ATestFullSubstate::SubstateA_Enter() {
	UE_LOG(LogFSM, Log, TEXT("Substate_A1_Enter"));
	if(Logger) {
		Logger->Log(TEXT("Substate_A1_Enter"));
	}
}
void ATestFullSubstate::SubstateA_Tick() {
	UE_LOG(LogFSM, Log, TEXT("Substate_A1_Tick"));
	if(Logger) {
		Logger->Log(TEXT("Substate_A1_Tick"));
	}
}
void ATestFullSubstate::SubstateA_Exit() {
	UE_LOG(LogFSM, Log, TEXT("Substate_A1_Exit"));
	if(Logger) {
		Logger->Log(TEXT("Substate_A1_Exit"));
	}
}
void ATestFullSubstate::SubstateB_Enter() {
	UE_LOG(LogFSM, Log, TEXT("Substate_B1_Enter"));
	if(Logger) {
		Logger->Log(TEXT("Substate_B1_Enter"));
	}
}
void ATestFullSubstate::SubstateB_Tick() {
	UE_LOG(LogFSM, Log, TEXT("Substate_B1_Tick"));
	if(Logger) {
		Logger->Log(TEXT("Substate_B1_Tick"));
	}
}
void ATestFullSubstate::SubstateB_Exit() {
	UE_LOG(LogFSM, Log, TEXT("Substate_B1_Exit"));
	if(Logger) {
		Logger->Log(TEXT("Substate_B1_Exit"));
	}
}


/*
ATestPingPongPause
*/


FTestSignature RunEnterDelegate;
FTestSignature PauseEnterDelegate;
FTestSignature SubstatePingEnterDelegate;
FTestSignature SubstatePongEnterDelegate;

ATestPingPongPause::ATestPingPongPause() :
	ShouldRun(true) {
	RunEnterDelegate.BindUObject(this, &ATestPingPongPause::Run_Enter);
	PauseEnterDelegate.BindUObject(this, &ATestPingPongPause::Pause_Enter);
	SubstatePingEnterDelegate.BindUObject(this, &ATestPingPongPause::Ping_Enter);
	SubstatePongEnterDelegate.BindUObject(this, &ATestPingPongPause::Pong_Enter);

	// Run
	auto& pingPongMachine = AddSubmachineState<EActorTestSubstatePingPong>(EActorTestState::Run, &RunEnterDelegate, nullptr, nullptr);
	AddSubstate<EActorTestSubstatePingPong, FTestSignature>(pingPongMachine, EActorTestSubstatePingPong::Substate_Ping, &SubstatePingEnterDelegate, nullptr, nullptr);
	AddSubstate<EActorTestSubstatePingPong, FTestSignature>(pingPongMachine, EActorTestSubstatePingPong::Substate_Pong, &SubstatePongEnterDelegate, nullptr, nullptr);

	// Pause
	AddState(EActorTestState::Pause, nullptr, &PauseEnterDelegate, nullptr);

	ChangeState(EActorTestState::Run);
}

void ATestPingPongPause::Run_Enter() {
	UE_LOG(LogFSM, Log, TEXT("Run_Enter"));
	if(Logger) {
		Logger->Log(TEXT("Run_Enter"));
	}
}

void ATestPingPongPause::Run_Tick() {
	if(!ShouldRun) {
		ChangeState(EActorTestState::Pause);
	}
}

void ATestPingPongPause::Pause_Enter() {
	UE_LOG(LogFSM, Log, TEXT("Pause_Enter"));
	if(Logger) {
		Logger->Log(TEXT("Pause_Enter"));
	}
}

void ATestPingPongPause::Pause_Tick() {
	if(ShouldRun) {
		ChangeState(EActorTestState::Run);
	}
}

void ATestPingPongPause::Ping_Enter() {
	UE_LOG(LogFSM, Log, TEXT("Ping"));
	if(Logger) {
		Logger->Log(TEXT("Ping"));
	}
	ChangeState(EActorTestState::Pong);
}

void ATestPingPongPause::Pong_Enter() {
	UE_LOG(LogFSM, Log, TEXT("Pong"));
	if(Logger) {
		Logger->Log(TEXT("Pong"));
	}
	ChangeState(EActorTestState::Ping);
}
