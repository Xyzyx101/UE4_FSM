#pragma once

#include "CoreMinimal.h"
#include "StateMachine.h"
#include "Templates/UniquePtr.h"
#include "StateTestLogger.h"
#include "TestActors.generated.h"

UENUM()
enum class EActorTestState : uint8
{
	State1,
	State2,
	State3,
	Run,
	Pause,
	Ping,
	Pong,
	Substate_A1,
	Substate_A2,
	Substate_A3,
	Substate_A4,
	Substate_B1,
	Substate_B2,
	Substate_B3,
	Substate_B4,
	Substate_Ping,
	Substate_Pong
};

DECLARE_DELEGATE(FTestSignature);
/*
	Adding this FSM_API export macro is only here to force instantiation of all the templated functions
*/
UCLASS(Blueprintable)
class FSM_API ATestActorBase : public AActor, public StateMachine<EActorTestState, FTestSignature>
{
	GENERATED_BODY()

public:
	ATestActorBase();
	void Tick(float DeltaTime) override;
	void TestChangeState(EActorTestState newState) { ChangeState(newState); }
	FStateTestLogger<FString>* RegisterTestRunner(class FAutomationTestBase* testRunner);
	TUniquePtr<FStateTestLogger<FString>> Logger;
};

UCLASS(Blueprintable)
class ATestEnter : public ATestActorBase
{
	GENERATED_BODY()

public:
	ATestEnter();
	
private:
	FTestSignature State1EnterDelegate;
	FTestSignature State2EnterDelegate;

	UFUNCTION()
	void State1_Enter();
	UFUNCTION()
	void State2_Enter();
};

UCLASS(Blueprintable)
class ATestInitialState : public ATestActorBase
{
	GENERATED_BODY()

public:
	ATestInitialState();

private:
	FTestSignature State1EnterDelegate;
	FTestSignature State2EnterDelegate;

	UFUNCTION()
	void State1_Enter();
	UFUNCTION()
	void State2_Enter();
};

UCLASS(Blueprintable)
class ATestEnterTickExit : public ATestActorBase
{
	GENERATED_BODY()

public:
	ATestEnterTickExit();

private:
	FTestSignature State1EnterDelegate;
	FTestSignature State1TickDelegate;
	FTestSignature State1ExitDelegate;
	FTestSignature State2EnterDelegate;
	FTestSignature State2TickDelegate;
	FTestSignature State2ExitDelegate;

	UFUNCTION()
	void State1_Enter();
	UFUNCTION()
	void State1_Tick();
	UFUNCTION()
	void State1_Exit();
	UFUNCTION()
	void State2_Enter();
	UFUNCTION()
	void State2_Tick();
	UFUNCTION()
	void State2_Exit();
};

UCLASS(Blueprintable)
class ATestSequence : public ATestActorBase
{
	GENERATED_BODY()

public:
	ATestSequence();

private:
	FTestSignature State1EnterDelegate;
	FTestSignature State1TickDelegate;
	FTestSignature State2EnterDelegate;
	FTestSignature State3EnterDelegate;

	UFUNCTION()
	void State1_Enter();
	UFUNCTION()
	void State1_Tick();
	UFUNCTION()
	void State2_Enter();
	UFUNCTION()
	void State3_Enter();
};

UCLASS(Blueprintable)
class ATestPause : public ATestActorBase
{
	GENERATED_BODY()

public:
	ATestPause();

private:
	FTestSignature RunTickDelegate;
	FTestSignature PauseEnterDelegate;
	FTestSignature PauseExitDelegate;

	UFUNCTION()
	void Run_Tick();
	UFUNCTION()
	void Pause_Enter();
	UFUNCTION()
	void Pause_Exit();
};

UCLASS(Blueprintable)
class APingPong : public ATestActorBase
{
	GENERATED_BODY()

public:
	APingPong();
private:
	FTestSignature PingEvent;
	FTestSignature PongEvent;

	UFUNCTION()
	void Ping();
	UFUNCTION()
	void Pong();
};

UCLASS(Blueprintable)
class ATestEnterSubstate : public ATestActorBase
{
	GENERATED_BODY()

public:
	ATestEnterSubstate();

private:
	FTestSignature State1EnterDelegate;
	FTestSignature State2EnterDelegate;
	FTestSignature Substate1EnterDelegate;
	FTestSignature Substate2EnterDelegate;

	UFUNCTION()
	void State1_Enter();
	UFUNCTION()
	void State2_Enter();
	UFUNCTION()
	void Substate1_Enter();
	UFUNCTION()
	void Substate2_Enter();
};

UCLASS(Blueprintable)
class ATestFullSubstate : public ATestActorBase
{
	GENERATED_BODY()

public:
	ATestFullSubstate();

private:
	FTestSignature State1EnterDelegate;
	FTestSignature State1TickDelegate;
	FTestSignature State1ExitDelegate;
	FTestSignature State2EnterDelegate;
	FTestSignature State2TickDelegate;
	FTestSignature State2ExitDelegate;
	FTestSignature SubstateAEnterDelegate;
	FTestSignature SubstateATickDelegate;
	FTestSignature SubstateAExitDelegate;
	FTestSignature SubstateBEnterDelegate;
	FTestSignature SubstateBTickDelegate;
	FTestSignature SubstateBExitDelegate;

	UFUNCTION()
	void State1_Enter();
	UFUNCTION()
	void State1_Tick();
	UFUNCTION()
	void State1_Exit();
	UFUNCTION()
	void State2_Enter();
	UFUNCTION()
	void State2_Tick();
	UFUNCTION()
	void State2_Exit();
	
	UFUNCTION()
	void SubstateA_Enter();
	UFUNCTION()
	void SubstateA_Tick();
	UFUNCTION()
	void SubstateA_Exit();
	UFUNCTION()
	void SubstateB_Enter();
	UFUNCTION()
	void SubstateB_Tick();
	UFUNCTION()
	void SubstateB_Exit();
};

UCLASS(Blueprintable)
class ATestPingPongPause : public ATestActorBase
{
	GENERATED_BODY()

public:
	ATestPingPongPause();

	/* Public interface to start or stop */
	void PingPong(bool shouldRun) { bShouldRun = shouldRun; };

private:
	UFUNCTION()
	void Run_Enter();
	UFUNCTION()
	void Run_Tick();
	UFUNCTION()
	void Pause_Enter();
	UFUNCTION()
	void Pause_Tick();
	UFUNCTION()
	void Ping_Enter();
	UFUNCTION()
	void Pong_Enter();

	FTestSignature RunEnterDelegate;
	FTestSignature RunTickDelegate;
	FTestSignature PauseEnterDelegate;
	FTestSignature PauseTickDelegate;
	FTestSignature SubstatePingEnterDelegate;
	FTestSignature SubstatePongEnterDelegate;
	bool bShouldRun;
};
