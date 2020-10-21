#pragma once

template<typename ValueType>
struct FStateTestLogger
{
	TArray<ValueType> ExpectedValues;
	TArray<ValueType> LoggedValues;
	FAutomationTestBase* TestRunner;

	FStateTestLogger(FAutomationTestBase* InTestRunner = nullptr) : TestRunner(InTestRunner) {}

	/*~FStateTestLogger() {
		CheckAllLogged();
	}*/

	void Log(const ValueType& Value) {
		LoggedValues.Add(Value);
		if(ExpectedValues.Num() > 0 && TestRunner != nullptr) {
			if(LoggedValues.Num() <= ExpectedValues.Num()) {
				FString logged = LoggedValues.Top();
				FString expected = ExpectedValues[LoggedValues.Num() - 1];
				TestRunner->AddInfo(FString::Printf(TEXT("Logged => '%s'   Expected => '%s'"), *logged, *expected));
				TestRunner->TestEqual("Logged value different then expected!", logged, expected);
			} else {
				TestRunner->TestTrue(FString::Printf(TEXT("Extra value => '%s'"), *LoggedValues.Top()), false);
			}
		}
	}

	void HasMetExpectedLogs() {
		TestRunner->AddInfo(FString::Printf(TEXT("Logged Count=> %d   Expected Count => %d"), LoggedValues.Num(), ExpectedValues.Num()));
		TestRunner->TestTrue("All values are logged", ExpectedValues.Num() == 0 || LoggedValues.Num() == ExpectedValues.Num());
	}
};