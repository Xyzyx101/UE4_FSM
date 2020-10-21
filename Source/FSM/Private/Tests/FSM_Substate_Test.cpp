#include "FSM.h"
#include "Misc/AssertionMacros.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "StateTestLogger.h"
#include "PingPong.h"
#include "TestActors.h"
#include "FSM_Test_Common.h"

#if WITH_DEV_AUTOMATION_TESTS

using namespace FSM_TEST;

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FTestEnterSubstate, ATestEnterSubstate*, Actor, EActorTestState, NewState);
bool FTestEnterSubstate::Update() {
	Actor->TestChangeState(NewState);
	return true;
}

//IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEnterTest, "FSM.Enter",
//(EAutomationTestFlags::ApplicationContextMask & ~EAutomationTestFlags::CommandletContext)
//| EAutomationTestFlags::ProductFilter)
//bool FEnterTest::RunTest(const FString& Parameters) {
//	AutomationOpenMap(TEXT("/FSM/Test/TestMap"));
//	UWorld* world = GetAnyGameWorld();
//	ATestEnter* enterActor = world->SpawnActor<ATestEnter>();
//	FStateTestLogger<FString>* logger = enterActor->RegisterTestRunner(this);
//	logger->ExpectedValues = TArray<FString>{ TEXT("State1_Enter"), TEXT("State2_Enter"), TEXT("State1_Enter") };
//	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(3));
//	ADD_LATENT_AUTOMATION_COMMAND(FTestEnterChangeState(enterActor, EActorTestState::State2));
//	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(3));
//	ADD_LATENT_AUTOMATION_COMMAND(FTestEnterChangeState(enterActor, EActorTestState::State1));
//	ADD_LATENT_AUTOMATION_COMMAND(FWaitForTick(1));
//	ADD_LATENT_AUTOMATION_COMMAND(FCheckLogs(logger));
//	ADD_LATENT_AUTOMATION_COMMAND(FDestroyActor(enterActor));
//	return true;
//}


#endif //WITH_DEV_AUTOMATION_TESTS