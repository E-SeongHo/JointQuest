// Fill out your copyright notice in the Description page of Project Settings.

#include "PythonLauncher.h"

//DEFINE_LOG_CATEGORY(LogCustom);

// Sets default values
UPythonLauncher::UPythonLauncher(const FObjectInitializer& init) : UActorComponent(init)
{
}


void UPythonLauncher::Launch(FString LaunchPath)
{
	FString command = FString(TEXT("python \""));
	command.Append(LaunchPath);
	command.Append("\"");

	auto res = Async(EAsyncExecution::Thread, [&, command]() {
		system(TCHAR_TO_UTF8(*command));
		AsyncTask(ENamedThreads::GameThread, [&]()
			{
				OnScriptEnd.Broadcast();
			});
		});
	OnScriptBegin.Broadcast();
	//UE_LOG(LogCustom, Log, TEXT("EXEC %s"), *command);
}

FString UPythonLauncher::GetRootPath()
{
	return SearchFromSource ? FPaths::GameSourceDir() : FPlatformProcess::BaseDir();
}

void UPythonLauncher::Launch()
{

	FString path = GetRootPath();
	path.Append(Path);
	//UE_LOG(LogCustom, Log, TEXT("Path is %s"), *path);
	Launch(path);
}

// Called when the game starts or when spawned
void UPythonLauncher::BeginPlay()
{
	Super::BeginPlay();
	if (LaunchImmediately) {
		Launch();
	}
}

