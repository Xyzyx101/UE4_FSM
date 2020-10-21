
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFSM, Log, All);

class FFSM : public FDefaultGameModuleImpl
{
public:

	static inline FFSM& Get() {
		return FModuleManager::LoadModuleChecked<FFSM>("FSM");
	}
	static inline bool IsAvailable() {
		return FModuleManager::Get().IsModuleLoaded("FSM");
	}

private:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};