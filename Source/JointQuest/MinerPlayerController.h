// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MinerPlayerController.generated.h"

UENUM(BlueprintType)
enum class EJointTrackingStatus : uint8
{
	Standing,
	Rising, // When the knee is raised above the LowerBoundRate
	Holding,
	Falling // When the knee is lowered below the UpperBoundRate
};

UCLASS()
class JOINTQUEST_API AMinerPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMinerPlayerController();

	virtual void PlayerTick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	EJointTrackingStatus GetCurrentStatus() const;

private:
	void ProcessKneeTracking();
	
private:
	UPROPERTY(EditAnyWhere)
	TSubclassOf<UUserWidget> GamePlayWidget;
	
	// Moves Score Logic to the GameMode 
	float PlayerScore = 0.0f;

	EJointTrackingStatus CurrentStatus;
	// joint tracking data

	// thresholds to play animation
	float UpperBoundRate = 0.7f;
	float LowerBoundRate = 0.7f;
	
	float HoldingTime = 1.0f;
};
