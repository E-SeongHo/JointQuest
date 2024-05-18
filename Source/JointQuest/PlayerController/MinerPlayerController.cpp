// Fill out your copyright notice in the Description page of Project Settings.

#include "MinerPlayerController.h"

#include "../GameplayWidget.h"
#include "../NetworkHandler.h"
#include "Blueprint/UserWidget.h"

AMinerPlayerController::AMinerPlayerController()
{
	CurrentStatus = EJointTrackingStatus::Standing;
	
}

void AMinerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	NetWorkHandler = NewObject<UNetworkHandler>(this);

	if(NetWorkHandler->Connect(TEXT("127.0.0.1"), 7777))
	{
		UE_LOG(LogTemp, Display, TEXT("connected to the socket"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("failed to connect socket"));
	}
	
	UUserWidget* GameplayWidget = CreateWidget(this, GamePlayWidget);
	if(GameplayWidget != nullptr) GameplayWidget->AddToViewport();
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


EJointTrackingStatus AMinerPlayerController::GetCurrentStatus() const
{
	return CurrentStatus;
}

void AMinerPlayerController::ProcessKneeTracking()
{
	//NetWorkHandler->ReceiveData();

	
}
