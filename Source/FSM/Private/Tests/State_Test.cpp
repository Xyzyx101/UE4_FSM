#include "FSM.h"
#include "Misc/AssertionMacros.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Misc/DateTime.h"
#include "State.h"
#include "PingPong.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace {
	enum class ETestState : uint8
	{
		TEST
	};
	DECLARE_DELEGATE(FDelegateTestSignature);
	DECLARE_EVENT(AActor, FEventTestSignature);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FStateTest, "FSM.State.Basic",
(EAutomationTestFlags::ApplicationContextMask & ~EAutomationTestFlags::CommandletContext)
| EAutomationTestFlags::ProductFilter)
bool FStateTest::RunTest(const FString& Parameters) {
	{
		// Single Delegate
		FDelegateTestSignature enter;
		enter.BindLambda([this]() {	AddWarning("Enter"); });
		FDelegateTestSignature tick;
		tick.BindLambda([this]() { AddWarning("Tick"); });
		FDelegateTestSignature exit;
		exit.BindLambda([this]() { AddWarning("Exit"); });
		State<ETestState, FDelegateTestSignature> testState{ ETestState::TEST, &enter, &tick, &exit };

		AddExpectedError(TEXT("Enter"), EAutomationExpectedErrorFlags::Exact, 1);
		testState.Enter();
		AddExpectedError(TEXT("Tick"), EAutomationExpectedErrorFlags::Exact, 1);
		testState.Tick();
		AddExpectedError(TEXT("Exit"), EAutomationExpectedErrorFlags::Exact, 1);
		testState.Exit();
		TestTrue("Delegates fired", HasMetExpectedErrors());

		State<ETestState, FDelegateTestSignature> emptyState{ ETestState::TEST, nullptr, nullptr, nullptr };
		emptyState.Enter();
		emptyState.Tick();
		emptyState.Exit();
		TestTrue("Unbound delegates should not cause a problem", true);
	}

	{
		std::enable_if<IsMulticastDelegate<FEventTestSignature>::value>::type;
		// Events
		FEventTestSignature enterEvent;
		enterEvent.AddLambda([this]() {	AddWarning("Enter Event"); });
		FEventTestSignature tickEvent;
		tickEvent.AddLambda([this]() { AddWarning("Tick Event"); });
		FEventTestSignature exitEvent;
		exitEvent.AddLambda([this]() { AddWarning("Exit Event"); });
		State<ETestState, FEventTestSignature> testEventState{ ETestState::TEST, &enterEvent, &tickEvent, &exitEvent };

		AddExpectedError(TEXT("Enter Event"), EAutomationExpectedErrorFlags::Exact, 1);
		testEventState.Enter();
		AddExpectedError(TEXT("Tick Event"), EAutomationExpectedErrorFlags::Exact, 1);
		testEventState.Tick();
		AddExpectedError(TEXT("Exit Event"), EAutomationExpectedErrorFlags::Exact, 1);
		testEventState.Exit();
		TestTrue("Events fired", HasMetExpectedErrors());

		State<ETestState, FEventTestSignature> emptyEventState{ ETestState::TEST, nullptr, nullptr, nullptr };
		emptyEventState.Enter();
		emptyEventState.Tick();
		emptyEventState.Exit();
		TestTrue("Unbound events should not cause a problem", true);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FStatePerfTest, "FSM.State.Performance",
(EAutomationTestFlags::ApplicationContextMask & ~EAutomationTestFlags::CommandletContext)
| EAutomationTestFlags::PerfFilter)
bool FStatePerfTest::RunTest(const FString& Parameters) {
	const int TARGET_ITERATIONS_PER_MILLISECOND = 25000;
	const int TARGET_MILLISECONDS = 100;
	const int ITERATIONS = TARGET_ITERATIONS_PER_MILLISECOND * TARGET_MILLISECONDS;
	const FTimespan targetTime{ TARGET_MILLISECONDS * ETimespan::TicksPerMillisecond };
	{
		int enterTest = 0;
		int tickTest = 0;
		int exitTest = 0;
		// Single Delegate
		FDelegateTestSignature enter;
		enter.BindLambda([&enterTest]() { ++enterTest; });
		FDelegateTestSignature tick;
		tick.BindLambda([&tickTest]() { ++tickTest; });
		FDelegateTestSignature exit;
		exit.BindLambda([&exitTest]() { ++exitTest; });
		State<ETestState, FDelegateTestSignature> perfState{ ETestState::TEST, &enter, &tick, &exit };

		FDateTime startTime = FDateTime::UtcNow();
		for(int i = 0; i < ITERATIONS; ++i) {
			perfState.Enter();
			perfState.Tick();
			perfState.Exit();
		}
		FTimespan testTime = FDateTime::UtcNow() - startTime;
		AddInfo(FString::Printf(TEXT("Delegate Iterations:%d   TargetTime:%fs   ActualTime:%fs"), ITERATIONS, (float)targetTime.GetTotalMilliseconds() / 1000.f, (float)testTime.GetTotalMilliseconds() / 1000.f));
		TestTrue(TEXT("Enter should match"), enterTest == ITERATIONS);
		TestTrue(TEXT("Tick should match"), tickTest == ITERATIONS);
		TestTrue(TEXT("Exit should match"), exitTest == ITERATIONS);
		TestTrue(FString::Printf(TEXT("Should do %d iterations per millisecond"), TARGET_ITERATIONS_PER_MILLISECOND), testTime < targetTime);
	}
	{
		int enterTest = 0;
		int tickTest = 0;
		int exitTest = 0;
		// Event Delegate
		FEventTestSignature enter;
		enter.AddLambda([&enterTest]() { ++enterTest; });
		FEventTestSignature tick;
		tick.AddLambda([&tickTest]() { ++tickTest; });
		FEventTestSignature exit;
		exit.AddLambda([&exitTest]() { ++exitTest; });
		State<ETestState, FEventTestSignature> perfState{ ETestState::TEST, &enter, &tick, &exit };

		FDateTime startTime = FDateTime::UtcNow();
		for(int i = 0; i < ITERATIONS; ++i) {
			perfState.Enter();
			perfState.Tick();
			perfState.Exit();
		}
		FTimespan testTime = FDateTime::UtcNow() - startTime;
		AddInfo(FString::Printf(TEXT("Event Iterations:%d   TargetTime:%fs   ActualTime:%fs"), ITERATIONS, (float)targetTime.GetTotalMilliseconds() / 1000.f, (float)testTime.GetTotalMilliseconds() / 1000.f));
		TestTrue(TEXT("Enter should match"), enterTest == ITERATIONS);
		TestTrue(TEXT("Tick should match"), tickTest == ITERATIONS);
		TestTrue(TEXT("Exit should match"), exitTest == ITERATIONS);
		TestTrue(FString::Printf(TEXT("Should do %d event iterations per millisecond"), TARGET_ITERATIONS_PER_MILLISECOND), testTime < targetTime);
	}
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS