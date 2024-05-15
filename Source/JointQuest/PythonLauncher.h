// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PythonLauncher.generated.h"

//DECLARE_LOG_CATEGORY_EXTERN(LogCustom, Log, All);

UCLASS()
class JOINTQUEST_API APythonLauncher : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Python Launcher")
	FString Path;

	// Sets default values for this actor's properties
	APythonLauncher();

	void Launch(FString LaunchPath);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TFuture<void> LaunchThread;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
