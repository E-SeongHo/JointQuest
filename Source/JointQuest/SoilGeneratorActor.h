// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoilGeneratorActor.generated.h"

class AMovableActor;

UCLASS()
class JOINTQUEST_API ASoilGeneratorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASoilGeneratorActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void GenerateSoils();
	void RearrangeSoils();
	
public:
	TQueue<AMovableActor*> LeftSoilsQueue;
	TQueue<AMovableActor*> RightSoilsQueue;
	TQueue<AMovableActor*> BackSoilsQueue;

	int32 GeneratedBlocks = 0;
	
	const int32 GenAmount = 10;
	const int32 GenThreshold = 5; 
	
	FVector2D LeftGenPos;
	FVector2D RightGenPos;
	FVector2D BackGenPos;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Soil, meta = (AllowPrivateAccess = "true"))
	float BlockHeight = 2000.0f; // hack : for test

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Soil, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> CharacterStage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Soil, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMovableActor> LeftBlockTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Soil, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMovableActor> RightBlockTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Soil, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMovableActor> BackBlockTemplate;

	FVector RefLocation;

	AMovableActor* TailPtr = nullptr;

	float RearrangeHeight = -4000.0f;
};
