// Fill out your copyright notice in the Description page of Project Settings.

#include "PythonLauncher.h"

//DEFINE_LOG_CATEGORY(LogCustom);

// Sets default values
APythonLauncher::APythonLauncher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void APythonLauncher::Launch(FString LaunchPath) {
	FString command = FString(TEXT("python \""));
	command.Append(LaunchPath);
	command.Append("\"");
	system(TCHAR_TO_UTF8(*command));
	//UE_LOG(LogCustom, Log, TEXT("EXEC %s"), *command);
}

// Called when the game starts or when spawned
void APythonLauncher::BeginPlay()
{
	Super::BeginPlay();
	FString path = FPaths::GameSourceDir();//FPlatformProcess::BaseDir();
	path.Append(Path);
	//UE_LOG(LogCustom, Log, TEXT("Path is %s"), *path);
	Launch(path);
}

// Called every frame
void APythonLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

