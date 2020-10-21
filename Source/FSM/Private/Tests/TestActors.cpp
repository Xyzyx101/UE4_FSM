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
ATestEnterSubstate
*/

ATestEnterSubstate::ATestEnterSubstate() {
	State1EnterDelegate.BindUObject(this, &ATestEnterSubstate::State1_Enter);
//	Substate<EActorTestState, FTestSignature> state1 = AddSubstate(EActorTestState::State1, &State1EnterDelegate, nullptr, nullptr);
	Substate1EnterDelegate.BindUObject(this, &ATestEnterSubstate::State1_Enter);
	//state1.AddState(EActorTestState::Substate1, &Substate1EnterDelegate, nullptr, nullptr);

	State2EnterDelegate.BindUObject(this, &ATestEnterSubstate::State1_Enter);
	//Substate<EActorTestState, FTestSignature> state2 = AddSubstate(EActorTestState::State1, &State1EnterDelegate, nullptr, nullptr);
	State2EnterDelegate.BindUObject(this, &ATestEnterSubstate::State1_Enter);
	//state2.AddState(EActorTestState::Substate2, &Substate2EnterDelegate, nullptr, nullptr);
	
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
	UE_LOG(LogFSM, Log, TEXT("Substate1_Enter"));
	if(Logger) {
		Logger->Log(TEXT("Substate1_Enter"));
	}
}

void ATestEnterSubstate::Substate2_Enter() {
	UE_LOG(LogFSM, Log, TEXT("Substate2_Enter"));
	if(Logger) {
		Logger->Log(TEXT("Substate2_Enter"));
	}
}
