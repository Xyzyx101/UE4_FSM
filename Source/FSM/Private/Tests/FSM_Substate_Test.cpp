#include "FSM.h"
#include "Misc/AssertionMacros.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "StateTestLogger.h"
#include "TestActors.h"
#include "FSM_Test_Common.h"

#if WITH_DEV_AUTOMATION_TESTS

using namespace FSM_TEST;

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FTestSubstateEnterChangeState, ATestEnterSubstate*, Actor, EActorTestState, NewState);
bool FTestSubstateEnterChangeState::Update() {
	Actor->TestChangeState(NewState);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSubstateEnterTest, "FSM.SubstateEnter",
	(EAutomationTestFlags::ApplicationContextMask & ~EAutomationTestFlags::CommandletContext)
	| EAutomationTestFlags::ProductFilter)
	bool FSubstateEnterTest::RunTest(const FString& Parameters) {
	AutomationOpenMap(TEXT("/FSM/Test/TestMap"));
	UWorld* world = GetAnyGameWorld();
	ATestEnterSubstate* substateActor = world->SpawnActor<ATestEnterSubstate>();
	FStateTestLogger<FString>* logger = substateActor->RegisterTestRunner(this);
	logger->ExpectedValues = TArray<FString>{
		TEXT("State1_Enter"),
		TEXT("Substate_A1_Enter"),
		TEXT("State2_Enter"),
		TEXT("Substate_B1_Enter"),
		TEXT("State1_Enter"),
		TEXT("Substate_A1_Enter"),
		TEXT("State2_Enter"),
		TEXT("Substate_B1_Enter"),
	};
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(1));
	ADD_LATENT_AUTOMATION_COMMAND(FTestSubstateEnterChangeState(substateActor, EActorTestState::State2));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(1));
	ADD_LATENT_AUTOMATION_COMMAND(FTestSubstateEnterChangeState(substateActor, EActorTestState::State1));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(3));
	ADD_LATENT_AUTOMATION_COMMAND(FTestSubstateEnterChangeState(substateActor, EActorTestState::State2));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(3));
	ADD_LATENT_AUTOMATION_COMMAND(FCheckLogs(logger));
	ADD_LATENT_AUTOMATION_COMMAND(FDestroyActor(substateActor));
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FTestSubstateFullChangeState, ATestFullSubstate*, Actor, EActorTestState, NewState);
bool FTestSubstateFullChangeState::Update() {
	Actor->TestChangeState(NewState);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSubstateFullTest, "FSM.SubstateFull",
	(EAutomationTestFlags::ApplicationContextMask & ~EAutomationTestFlags::CommandletContext)
	| EAutomationTestFlags::ProductFilter)
	bool FSubstateFullTest::RunTest(const FString& Parameters) {
	AutomationOpenMap(TEXT("/FSM/Test/TestMap"));
	UWorld* world = GetAnyGameWorld();
	ATestFullSubstate* substateActor = world->SpawnActor<ATestFullSubstate>();
	FStateTestLogger<FString>* logger = substateActor->RegisterTestRunner(this);
	logger->ExpectedValues = TArray<FString>{
		TEXT("State1_Enter"),
		TEXT("State1_Tick"),
		TEXT("Substate_A1_Enter"),
		TEXT("Substate_A1_Tick"),
		TEXT("Substate_A1_Exit"),
		TEXT("State1_Exit"),

		TEXT("State2_Enter"),
		TEXT("State2_Tick"),
		TEXT("Substate_B1_Enter"),
		TEXT("Substate_B1_Tick"),
		TEXT("Substate_B1_Exit"),
		TEXT("State2_Exit"),

		TEXT("State1_Enter"),
		TEXT("State1_Tick"),
		TEXT("Substate_A1_Enter"),
		TEXT("Substate_A1_Tick"),
		TEXT("State1_Tick"),
		TEXT("Substate_A1_Tick"),
		TEXT("State1_Tick"),
		TEXT("Substate_A1_Tick"),
		TEXT("Substate_A1_Exit"),
		TEXT("State1_Exit"),

		TEXT("State2_Enter"),
		TEXT("State2_Tick"),
		TEXT("Substate_B1_Enter"),
		TEXT("Substate_B1_Tick"),
		TEXT("State2_Tick"),
		TEXT("Substate_B1_Tick"),
		TEXT("State2_Tick"),
		TEXT("Substate_B1_Tick"),
	};
	ADD_LATENT_AUTOMATION_COMMAND(FTestSubstateFullChangeState(substateActor, EActorTestState::State2));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(1));
	ADD_LATENT_AUTOMATION_COMMAND(FTestSubstateFullChangeState(substateActor, EActorTestState::State1));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(3));
	ADD_LATENT_AUTOMATION_COMMAND(FTestSubstateFullChangeState(substateActor, EActorTestState::State2));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(3));
	ADD_LATENT_AUTOMATION_COMMAND(FCheckLogs(logger));
	ADD_LATENT_AUTOMATION_COMMAND(FDestroyActor(substateActor));
	return true;
}

// initial state is enum 0?  or changestate?
// ChangeState
// Substates
// change substate
// change superstate


#endif //WITH_DEV_AUTOMATION_TESTS