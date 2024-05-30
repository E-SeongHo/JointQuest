// Fill out your copyright notice in the Description page of Project Settings.

#include "MinerPlayerController.h"

#include "../Widget/GameplayWidget.h"
#include "../NetworkHandler.h"
#include "Blueprint/UserWidget.h"
#include "../Widget/GraphWidget.h"
#include "JointQuest/Actor/ScoreComponent.h"
#include "JointQuest/TransportManager.h"

AMinerPlayerController::AMinerPlayerController()
{
	CurrentStatus = EJointTrackingStatus::Standing;
	ScoreComp = CreateDefaultSubobject<UScoreComponent>(TEXT("ScoreComponent"));
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
	
	/*
	// temporary simulation code
	static float ElapsedTime = 0.0f;
	ElapsedTime += DeltaSeconds;

	// reads data and sets status
	/*if(ElapsedTime > 5.0f)
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
	*/
}

EJointTrackingStatus AMinerPlayerController::GetCurrentStatus() const
{
	return CurrentStatus;
}

float AMinerPlayerController::GetPeakAngle() const
{
	return PlayerPeakAngle;
}

UScoreComponent* AMinerPlayerController::GetScoreComponent() const
{
	return ScoreComp;
}

void AMinerPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	UGraphWidget* RecordWidget = Cast<UGraphWidget>(CreateWidget(this, RecordChartWidget));
	if(RecordWidget != nullptr)
	{
		TArray<FExerciseRecord> Records = ScoreComp->GetAllRecords();
		RecordWidget->Records = Records;

		int32 x, y;
		GetViewportSize(x, y);
		RecordWidget->ViewPortSize = FVector2d(x, y);
		RecordWidget->Stride = static_cast<float>(x) / Records.Num();
		RecordWidget->AddToViewport(999);
	}
}

void AMinerPlayerController::ProcessKneeTracking()
{
	//NetWorkHandler->ReceiveData();
	const float CurrentAngle = GetCurrentAngle();
	const float RaisedRate = CurrentAngle / PlayerLimitAngle;

	PlayerSubAngle1 += 0.1f;
	if(PlayerSubAngle1 >= 90.0f)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), AngleOutOfBoundWarningWidget);
		if(Widget != nullptr)
		{
			Widget->AddToViewport();
		}
		PlayerSubAngle1 = 0.0f;
	}
	
	if (CurrentStatus == EJointTrackingStatus::Standing)
	{
		PlayerPeakAngle = 0.0f;
		if(RaisedRate > LowerBoundRate)
		{
			CurrentStatus = EJointTrackingStatus::Rising; 
		}
	}
	else if(CurrentStatus == EJointTrackingStatus::Rising)
	{
		if(RaisedRate >= UpperBoundRate)
		{
			CurrentStatus = EJointTrackingStatus::Holding;
		}
		else if(RaisedRate <= LowerBoundRate)
		{
			CurrentStatus = EJointTrackingStatus::Standing;
		}
	}
	else if(CurrentStatus == EJointTrackingStatus::Holding)
	{
		PlayerPeakAngle = FMath::Max(PlayerPeakAngle, CurrentAngle);
		
		if(RaisedRate < UpperBoundRate)
		{
			CurrentStatus = EJointTrackingStatus::Falling;
		}
	}
	else if(CurrentStatus == EJointTrackingStatus::Falling)
	{
		if(RaisedRate <= LowerBoundRate)
		{
			CurrentStatus = EJointTrackingStatus::Standing;
		}
	}
}

float AMinerPlayerController::GetCurrentAngle()
{
	// Json parsing
	return ATransportManager::GetJointAngle();//0.0f;
}  
