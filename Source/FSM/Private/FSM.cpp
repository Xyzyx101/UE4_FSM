#include "FSM.h"

DEFINE_LOG_CATEGORY(LogFSM)

void FFSM::StartupModule() {
	UE_LOG(LogFSM, Log, TEXT("Startup FSM module"));
}

void FFSM::ShutdownModule() {
	UE_LOG(LogFSM, Log, TEXT("Shutdown FSM module"));
}

IMPLEMENT_GAME_MODULE(FFSM, FSM)
