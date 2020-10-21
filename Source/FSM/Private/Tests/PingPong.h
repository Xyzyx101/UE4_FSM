#pragma once

#include "CoreMinimal.h"
#include "StateMachine.h"
#include "Templates/UniquePtr.h"
#include "StateTestLogger.h"
#include "PingPong.generated.h"

UENUM()
enum class EPingPongState : uint8
{
	PING,
	PONG
};

DECLARE_DELEGATE(FPingPongStateSignature);

UCLASS(Blueprintable)
class APingPong : public AActor, public StateMachine<EPingPongState, FPingPongStateSignature>
{
	GENERATED_BODY()

public:
	APingPong();
	virtual void Tick(float DeltaTime) override;
private:
	FPingPongStateSignature PingEvent;
	FPingPongStateSignature PongEvent;

	UFUNCTION()
	void Ping();
	UFUNCTION()
	void Pong();

public:
	FStateTestLogger<FString>* RegisterTestRunner(class FAutomationTestBase* testRunner);
	TUniquePtr<FStateTestLogger<FString>> Logger;
};
