#include "FSM.h"
#include "Misc/AssertionMacros.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "StateTestLogger.h"
#include "TestActors.h"
#include "FSM_Test_Common.h"

#if WITH_DEV_AUTOMATION_TESTS

using namespace FSM_TEST;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPingPongTest, "FSM.PingPong",
(EAutomationTestFlags::ApplicationContextMask & ~EAutomationTestFlags::CommandletContext)
| EAutomationTestFlags::ProductFilter)
bool FPingPongTest::RunTest(const FString& Parameters) {
	AutomationOpenMap(TEXT("/FSM/Test/TestMap"));
	UWorld* world = GetAnyGameWorld();
	APingPong* pingPong = world->SpawnActor<APingPong>();
	FStateTestLogger<FString>* logger = pingPong->RegisterTestRunner(this);
	logger->ExpectedValues = TArray<FString>{ TEXT("Ping"), TEXT("Pong") ,TEXT("Ping") ,TEXT("Pong") };
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(3));
	ADD_LATENT_AUTOMATION_COMMAND(FCheckLogs(logger));
	ADD_LATENT_AUTOMATION_COMMAND(FDestroyActor(pingPong));
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FTestEnterChangeState, ATestEnter*, Actor, EActorTestState, NewState);
bool FTestEnterChangeState::Update() {
	Actor->TestChangeState(NewState);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEnterTest, "FSM.Enter",
(EAutomationTestFlags::ApplicationContextMask & ~EAutomationTestFlags::CommandletContext)
| EAutomationTestFlags::ProductFilter)
bool FEnterTest::RunTest(const FString& Parameters) {
	AutomationOpenMap(TEXT("/FSM/Test/TestMap"));
	UWorld* world = GetAnyGameWorld();
	ATestEnter* enterActor = world->SpawnActor<ATestEnter>();
	FStateTestLogger<FString>* logger = enterActor->RegisterTestRunner(this);
	logger->ExpectedValues = TArray<FString>{ TEXT("State1_Enter"), TEXT("State2_Enter"), TEXT("State1_Enter") };
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(3));
	ADD_LATENT_AUTOMATION_COMMAND(FTestEnterChangeState(enterActor, EActorTestState::State2));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(3));
	ADD_LATENT_AUTOMATION_COMMAND(FTestEnterChangeState(enterActor, EActorTestState::State1));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(1));
	ADD_LATENT_AUTOMATION_COMMAND(FCheckLogs(logger));
	ADD_LATENT_AUTOMATION_COMMAND(FDestroyActor(enterActor));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEnterChangeIsIdempotentTest, "FSM.EnterChangeIsIdempotent",
(EAutomationTestFlags::ApplicationContextMask & ~EAutomationTestFlags::CommandletContext)
| EAutomationTestFlags::ProductFilter)
bool FEnterChangeIsIdempotentTest::RunTest(const FString& Parameters) {
	AutomationOpenMap(TEXT("/FSM/Test/TestMap"));
	UWorld* world = GetAnyGameWorld();
	ATestEnter* enterActor = world->SpawnActor<ATestEnter>();
	FStateTestLogger<FString>* logger = enterActor->RegisterTestRunner(this);
	logger->ExpectedValues = TArray<FString>{ TEXT("State1_Enter"), TEXT("State2_Enter"), TEXT("State1_Enter") };
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(3));
	ADD_LATENT_AUTOMATION_COMMAND(FTestEnterChangeState(enterActor, EActorTestState::State2));
	ADD_LATENT_AUTOMATION_COMMAND(FTestEnterChangeState(enterActor, EActorTestState::State2));
	ADD_LATENT_AUTOMATION_COMMAND(FTestEnterChangeState(enterActor, EActorTestState::State2));
	ADD_LATENT_AUTOMATION_COMMAND(FTestEnterChangeState(enterActor, EActorTestState::State2));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(3));
	ADD_LATENT_AUTOMATION_COMMAND(FTestEnterChangeState(enterActor, EActorTestState::State1));
	ADD_LATENT_AUTOMATION_COMMAND(FTestEnterChangeState(enterActor, EActorTestState::State1));
	ADD_LATENT_AUTOMATION_COMMAND(FTestEnterChangeState(enterActor, EActorTestState::State1));
	ADD_LATENT_AUTOMATION_COMMAND(FTestEnterChangeState(enterActor, EActorTestState::State1));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(1));
	ADD_LATENT_AUTOMATION_COMMAND(FCheckLogs(logger));
	ADD_LATENT_AUTOMATION_COMMAND(FDestroyActor(enterActor));
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FTestEnterTickExitChangeState, ATestEnterTickExit*, Actor, EActorTestState, NewState);
bool FTestEnterTickExitChangeState::Update() {
	Actor->TestChangeState(NewState);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEnterTickExitTest, "FSM.EnterTickExit",
(EAutomationTestFlags::ApplicationContextMask & ~EAutomationTestFlags::CommandletContext)
| EAutomationTestFlags::ProductFilter)
bool FEnterTickExitTest::RunTest(const FString& Parameters) {
	AutomationOpenMap(TEXT("/FSM/Test/TestMap"));
	UWorld* world = GetAnyGameWorld();
	ATestEnterTickExit* enterTickExitActor = world->SpawnActor<ATestEnterTickExit>();
	FStateTestLogger<FString>* logger = enterTickExitActor->RegisterTestRunner(this);
	logger->ExpectedValues = TArray<FString>{ 
		TEXT("State1_Enter"),
		TEXT("State1_Tick"), 
		TEXT("State1_Tick"),
		TEXT("State1_Tick"),
		TEXT("State1_Exit"),
		TEXT("State2_Enter"),
		TEXT("State2_Tick"),
		TEXT("State2_Tick"),
		TEXT("State2_Tick"),
		TEXT("State2_Exit"),
		TEXT("State1_Enter"),
		TEXT("State1_Tick"),
		TEXT("State1_Tick"),
		TEXT("State1_Tick"),
	};
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(2));
	ADD_LATENT_AUTOMATION_COMMAND(FTestEnterTickExitChangeState(enterTickExitActor, EActorTestState::State2));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(3));
	ADD_LATENT_AUTOMATION_COMMAND(FTestEnterTickExitChangeState(enterTickExitActor, EActorTestState::State1));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(3));
	ADD_LATENT_AUTOMATION_COMMAND(FCheckLogs(logger));
	ADD_LATENT_AUTOMATION_COMMAND(FDestroyActor(enterTickExitActor));
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FTestSequenceChangeState, ATestSequence*, Actor, EActorTestState, NewState);
bool FTestSequenceChangeState::Update() {
	Actor->TestChangeState(NewState);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequenceTest, "FSM.Sequence",
(EAutomationTestFlags::ApplicationContextMask & ~EAutomationTestFlags::CommandletContext)
| EAutomationTestFlags::ProductFilter)
bool FSequenceTest::RunTest(const FString& Parameters) {
	AutomationOpenMap(TEXT("/FSM/Test/TestMap"));
	UWorld* world = GetAnyGameWorld();
	ATestSequence* sequenceActor = world->SpawnActor<ATestSequence>();
	FStateTestLogger<FString>* logger = sequenceActor->RegisterTestRunner(this);
	logger->ExpectedValues = TArray<FString>{
		TEXT("State1_Enter"),
		TEXT("State1_Tick"),
		TEXT("State1_Tick"),
		TEXT("State1_Tick"),
		TEXT("State2_Enter"),
		TEXT("State3_Enter"),
		TEXT("State1_Enter"),
		TEXT("State1_Tick"),
		TEXT("State1_Tick"),
		TEXT("State1_Tick"),
		TEXT("State2_Enter"),
		TEXT("State3_Enter"),
		TEXT("State1_Enter"),
		TEXT("State1_Tick"),
		TEXT("State1_Tick"),
		TEXT("State1_Tick"),
	};
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(2));
	ADD_LATENT_AUTOMATION_COMMAND(FTestSequenceChangeState(sequenceActor, EActorTestState::State2));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(5));
	ADD_LATENT_AUTOMATION_COMMAND(FTestSequenceChangeState(sequenceActor, EActorTestState::State2));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(5));
	ADD_LATENT_AUTOMATION_COMMAND(FCheckLogs(logger));
	ADD_LATENT_AUTOMATION_COMMAND(FDestroyActor(sequenceActor));
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FTestPauseChangeState, ATestPause*, Actor, EActorTestState, NewState);
bool FTestPauseChangeState::Update() {
	Actor->TestChangeState(NewState);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseTest, "FSM.Pause",
(EAutomationTestFlags::ApplicationContextMask & ~EAutomationTestFlags::CommandletContext)
| EAutomationTestFlags::ProductFilter)
bool FPauseTest::RunTest(const FString& Parameters) {
	AutomationOpenMap(TEXT("/FSM/Test/TestMap"));
	UWorld* world = GetAnyGameWorld();
	ATestPause* pauseActor = world->SpawnActor<ATestPause>();
	FStateTestLogger<FString>* logger = pauseActor->RegisterTestRunner(this);
	logger->ExpectedValues = TArray<FString>{
		TEXT("Run_Tick"),
		TEXT("Run_Tick"),
		TEXT("Run_Tick"),
		TEXT("Pause_Enter"),
		TEXT("Pause_Exit"),
		TEXT("Run_Tick"),
		TEXT("Run_Tick"),
		TEXT("Run_Tick"),
	};
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(2));
	ADD_LATENT_AUTOMATION_COMMAND(FTestPauseChangeState(pauseActor, EActorTestState::Pause));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(5));
	ADD_LATENT_AUTOMATION_COMMAND(FTestPauseChangeState(pauseActor, EActorTestState::Run));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(3));
	ADD_LATENT_AUTOMATION_COMMAND(FCheckLogs(logger));
	ADD_LATENT_AUTOMATION_COMMAND(FDestroyActor(pauseActor));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInitialState, "FSM.InitialState",
	(EAutomationTestFlags::ApplicationContextMask & ~EAutomationTestFlags::CommandletContext)
	| EAutomationTestFlags::ProductFilter)
	bool FInitialState::RunTest(const FString& Parameters) {
	AutomationOpenMap(TEXT("/FSM/Test/TestMap"));
	UWorld* world = GetAnyGameWorld();

	ATestEnter* defaultInitialActor = world->SpawnActor<ATestEnter>();
	FStateTestLogger<FString>* defaultLogger = defaultInitialActor->RegisterTestRunner(this);

	ATestInitialState* initialStateActor = world->SpawnActor<ATestInitialState>();
	FStateTestLogger<FString>* initalStateLogger = initialStateActor->RegisterTestRunner(this);

	defaultLogger->ExpectedValues = TArray<FString>{ TEXT("State1_Enter") };
	initalStateLogger->ExpectedValues = TArray<FString>{ TEXT("State2_Enter") };

	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(1));
	ADD_LATENT_AUTOMATION_COMMAND(FCheckLogs(defaultLogger));
	ADD_LATENT_AUTOMATION_COMMAND(FCheckLogs(initalStateLogger));
	ADD_LATENT_AUTOMATION_COMMAND(FDestroyActor(defaultInitialActor));
	ADD_LATENT_AUTOMATION_COMMAND(FDestroyActor(initialStateActor));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS