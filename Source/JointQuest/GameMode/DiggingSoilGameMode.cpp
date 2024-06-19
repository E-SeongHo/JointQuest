
#include "DiggingSoilGameMode.h"

#include "JointQuest/Actor/ScoreComponent.h"
#include "JointQuest/PlayerController/MinerPlayerController.h"
#include "Kismet/GameplayStatics.h"

ADiggingSoilGameMode::ADiggingSoilGameMode()
{
}

void ADiggingSoilGameMode::BeginPlay()
{
	Super::BeginPlay();

	// ExecuteTrackingProcess();
}

void ADiggingSoilGameMode::ExecuteTrackingProcess()
{
	FString PythonExecutable = TEXT("python3"); 
	FString ScriptPath = FPaths::ProjectDir() / TEXT("Scripts/mediapipe.py"); 
	FString CommandLine = FString::Printf(TEXT("\"%s\""), *ScriptPath); 

	FProcHandle ProcHandle = FPlatformProcess::CreateProc(
		*PythonExecutable,
		*CommandLine,
		true,
		false,
		false,
		nullptr,
		0,
		nullptr,
		nullptr
		);
	
	if (ProcHandle.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("tracking process starts"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("failed to start tracking"));
	}
}

int32 ADiggingSoilGameMode::GetScore() const
{
	AMinerPlayerController* PC = Cast<AMinerPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	return PC->GetScoreComponent()->GetCurrentScore(); 
}
