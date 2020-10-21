#pragma once
#include "Misc/AssertionMacros.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace FSM_TEST {
	// Copied from AutomationCommon.cpp.  The todo is Epic's
	// @todo this is a temporary solution. Once we know how to get test's hands on a proper world
	// this function should be redone/removed
	UWorld* GetAnyGameWorld() {
		UWorld* TestWorld = nullptr;
		const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
		for(const FWorldContext& Context : WorldContexts) {
			if(((Context.WorldType == EWorldType::PIE) || (Context.WorldType == EWorldType::Game)) && (Context.World() != NULL)) {
				TestWorld = Context.World();
				break;
			}
		}

		return TestWorld;
	}
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FWaitForActorInitialized, AActor*, Actor);
bool FWaitForActorInitialized::Update() {
	return Actor->IsActorInitialized();
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FWaitForTick, int, Count);
bool FWaitForTick::Update() {
	UE_LOG(LogFSM, Log, TEXT("Count : %d"), Count);
	return Count-- <= 0;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FCheckLogs, FStateTestLogger<FString>*, Logger);
bool FCheckLogs::Update() {
	Logger->HasMetExpectedLogs();
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FDestroyActor, AActor*, Actor);
bool FDestroyActor::Update() {
	Actor->Destroy();
	return true;
}

#endif //WITH_DEV_AUTOMATION_TESTS