#include "PingPong.h"

APingPong::APingPong() :
	Logger(nullptr) {
	PrimaryActorTick.bCanEverTick = true;
	PingEvent.BindUObject(this, &APingPong::Ping);
	AddState(EPingPongState::PING, nullptr, &PingEvent, nullptr);
	PongEvent.BindUObject(this, &APingPong::Pong);
	AddState(EPingPongState::PONG, nullptr, &PongEvent, nullptr);
	ChangeState(EPingPongState::PING);
}

void APingPong::Tick(float DeltaTime) {
	TickStateMachine(DeltaTime);
	Super::Tick(DeltaTime);
}

void APingPong::Ping() {
	UE_LOG(LogFSM, Log, TEXT("Ping State"));
	if(Logger) {
		Logger->Log(TEXT("Ping"));
	}
	ChangeState(EPingPongState::PONG);
}

void APingPong::Pong() {
	UE_LOG(LogFSM, Log, TEXT("Pong State"));
	if(Logger) {
		Logger->Log(TEXT("Pong"));
	}
	ChangeState(EPingPongState::PING);
}

FStateTestLogger<FString>* APingPong::RegisterTestRunner(FAutomationTestBase* testRunner) {
	Logger = MakeUnique<FStateTestLogger<FString>>(testRunner);
	return Logger.Get();;
}

