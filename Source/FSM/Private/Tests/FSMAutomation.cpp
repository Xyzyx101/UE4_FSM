
#include "Misc/AssertionMacros.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Engine.h"
#include "EngineUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// Copy of the hidden method GetAnyGameWorld() in AutomationCommon.cpp.
// Marked as temporary there, hence, this one is temporary, too.
UWorld* GetTestWorld() {
	const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
	for(const FWorldContext& Context : WorldContexts) {
		if(((Context.WorldType == EWorldType::PIE) || (Context.WorldType == EWorldType::Game))
			&& (Context.World() != nullptr)) {
			return Context.World();
		}
	}

	return nullptr;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFSMAutomationTest, "FSM.Automation",
	EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::ClientContext |
	EAutomationTestFlags::ProductFilter)

	bool FFSMAutomationTest::RunTest(const FString& Parameters) {
	AutomationOpenMap(TEXT("/FSM/Test/TestMap"));

	UWorld* world = GetTestWorld();

	//TestTrue("GameMode class is set correctly",	world->GetAuthGameMode()->IsA<YourGameModeBase>());
	//TestTrue("Essential actor is spawned", TActorIterator<AMyEssentialActor>(world));

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS