// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MinerPlayerController.generated.h"

UENUM(BlueprintType)
enum class EJointTrackingStatus : uint8
{
	Standing, // <= LowerBoundRate
	Rising, // > LowerBoundRate
	Holding, // >= UpperBoundRate
	Falling // < UpperBoundRate
};

class UNetworkHandler;
class UScoreComponent;

UCLASS()
class JOINTQUEST_API AMinerPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMinerPlayerController();

	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaSeconds) override;
	EJointTrackingStatus GetCurrentStatus() const;
	float GetPeakAngle() const;

	UScoreComponent* GetScoreComponent() const;
	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;
	
private:
	void ProcessKneeTracking();
	float GetCurrentAngle();
	
private:
	UScoreComponent* ScoreComp;
	
	UPROPERTY(EditAnyWhere)
	TSubclassOf<UUserWidget> GamePlayWidget;

	UPROPERTY(EditAnyWhere)
	TSubclassOf<UUserWidget> RecordChartWidget;

	UPROPERTY(EditAnyWhere)
	TSubclassOf<UUserWidget> AngleOutOfBoundWarningWidget;
	
	UNetworkHandler* NetWorkHandler;

	// joint tracking data
	EJointTrackingStatus CurrentStatus;

	// player primary information
	float PlayerLimitAngle = 90.0f;
	float PlayerPeakAngle = 0.0f; // reset every reps

	// player secondary informaiton
	float PlayerSubAngle1 = 0.0f; // e.g. 허벅지와 종아리 안쪽 각도
	float PlayerSubAngle2 = 0.0f; // e.g. 골반과 허벅지의 수평방향 각도
	
	// thresholds to play animation
	float LowerBoundRate = 0.2f;
	float UpperBoundRate = 0.8f;
};
