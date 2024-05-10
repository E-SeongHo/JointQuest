// Fill out your copyright notice in the Description page of Project Settings.

#include "MinerPlayerController.h"

#include "Blueprint/UserWidget.h"

AMinerPlayerController::AMinerPlayerController()
{
	CurrentStatus = EJointTrackingStatus::Standing;
	
}



void AMinerPlayerController::PlayerTick(float DeltaSeconds)
{
	Super::PlayerTick(DeltaSeconds);

	// IPC ( with Mediapipe )
	// reads joint tracking data
	ProcessKneeTracking();
	
	// temporary simulation code
	static float ElapsedTime = 0.0f;
	ElapsedTime += DeltaSeconds;

	// reads data and sets status
	if(ElapsedTime > 5.0f)
	{
		ElapsedTime = 0.0f;
		CurrentStatus = EJointTrackingStatus::Rising;
	}

	if (CurrentStatus == EJointTrackingStatus::Rising && ElapsedTime >= 1.0f)
	{
		CurrentStatus = EJointTrackingStatus::Holding; 
		ElapsedTime = 0.0f; 
	}
	else if (CurrentStatus == EJointTrackingStatus::Holding && ElapsedTime >= 1.0f)
	{
		CurrentStatus = EJointTrackingStatus::Falling; 
		ElapsedTime = 0.0f; 
	}
	else if (CurrentStatus == EJointTrackingStatus::Falling && ElapsedTime >= 1.0f)
	{
		CurrentStatus = EJointTrackingStatus::Standing; 
		ElapsedTime = 0.0f;
	}
}

void AMinerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* GameplayWidget = CreateWidget(this, GamePlayWidget);
	if(GameplayWidget != nullptr) GameplayWidget->AddToViewport();
}

EJointTrackingStatus AMinerPlayerController::GetCurrentStatus() const
{
	return CurrentStatus;
}

void AMinerPlayerController::ProcessKneeTracking()
{
	// will be implemented
}
