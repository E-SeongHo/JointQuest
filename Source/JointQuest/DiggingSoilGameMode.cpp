
#include "DiggingSoilGameMode.h"

ADiggingSoilGameMode::ADiggingSoilGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/MiniGame_KneeRehab/BluePrints/BP_MinerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
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

void ADiggingSoilGameMode::AwardPoints(int32 Points)
{
	CurrentScore += Points;
}

int32 ADiggingSoilGameMode::GetScore(int32 Score) const
{
	return CurrentScore;
}
