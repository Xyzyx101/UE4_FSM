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
	Substate1,
	Substate2,
	Substate3,
	Substate4
};

DECLARE_DELEGATE(FTestSignature);

UCLASS(Blueprintable)
class ATestActorBase : public AActor, public StateMachine<EActorTestState, FTestSignature>
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