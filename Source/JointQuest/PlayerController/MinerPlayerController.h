// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MinerPlayerController.generated.h"

class UCaptureComponent;

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
class UWarningWidget;

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
	int32 GetFailedCnt() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	UScoreComponent* GetScoreComponent() const;
	UFUNCTION(BlueprintCallable, Category = "Capture")
	UCaptureComponent* GetCaptureComponent() const;
	
	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;
	
private:
	void ProcessKneeTracking();
	
private:
	UScoreComponent* ScoreComp;
	UCaptureComponent* CaptureComp;
	
	UPROPERTY(EditAnyWhere)
	TSubclassOf<UUserWidget> GraphWidgetClass;
	
	UPROPERTY(EditAnyWhere)
	TSubclassOf<UUserWidget> AngleOutOfBoundWarningWidget;

	bool bHasGameEnded = false;
	bool bHasGameStarted = false;
	
	// joint tracking data
	EJointTrackingStatus CurrentStatus;

	// player primary information
	float PlayerLimitAngle = 90.0f;
	float PlayerPeakAngle; // reset every reps
	float PlayerMainAngle;
	
	// player secondary information
	float PlayerSubAngle1 = 85.0f; // e.g. 허벅지와 종아리 안쪽 각도  70 <= x <= 100 
	float PlayerSubAngle2 = 100.0f; // e.g. 골반과 허벅지의 수평방향 각도  // 80 <= x <= 110
	int32 CntSubAngle1Failed = 0;
	int32 CntSubAngle2Failed = 0;
	UWarningWidget* WarningWidget;
	
	// thresholds to play animation
	float LowerBoundRate = 0.2f;
	float UpperBoundRate = 0.8f;
};
