// Fill out your copyright notice in the Description page of Project Settings.

#include "MinerPlayerController.h"

#include "../Widget/GameplayWidget.h"
#include "../NetworkHandler.h"
#include "Blueprint/UserWidget.h"
#include "../Widget/GraphWidget.h"
#include "JointQuest/Actor/ScoreComponent.h"
#include "JointQuest/TransportManager.h"
#include "JointQuest/Actor/CaptureComponent.h"
#include "JointQuest/Widget/WarningWidget.h"

AMinerPlayerController::AMinerPlayerController()
{
	CurrentStatus = EJointTrackingStatus::Standing;
	ScoreComp = CreateDefaultSubobject<UScoreComponent>(TEXT("ScoreComponent"));
	CaptureComp = CreateDefaultSubobject<UCaptureComponent>(TEXT("CaptureComponent"));
}

void AMinerPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	WarningWidget = CreateWidget<UWarningWidget>(GetWorld(), AngleOutOfBoundWarningWidget);
	WarningWidget->AddToViewport();
	WarningWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AMinerPlayerController::PlayerTick(float DeltaSeconds)
{
	Super::PlayerTick(DeltaSeconds);

	// IPC ( with Mediapipe )
	// reads joint tracking data
	ProcessKneeTracking();
}

EJointTrackingStatus AMinerPlayerController::GetCurrentStatus() const
{
	return CurrentStatus;
}

float AMinerPlayerController::GetPeakAngle() const
{
	return PlayerPeakAngle;
}

int32 AMinerPlayerController::GetFailedCnt() const
{
	return CntSubAngle1Failed + CntSubAngle2Failed;
}

UScoreComponent* AMinerPlayerController::GetScoreComponent() const
{
	return ScoreComp;
}

void AMinerPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	// Load new Level
	// And new level reads records from JointQuestGameInstance
	/*UGraphWidget* RecordWidget = Cast<UGraphWidget>(CreateWidget(this, RecordChartWidget));
	if(RecordWidget != nullptr)
	{
		TArray<FExerciseRecord> Records = ScoreComp->GetAllRecords();
		RecordWidget->Records = Records;

		int32 x, y;
		GetViewportSize(x, y);
		RecordWidget->ViewPortSize = FVector2d(x, y);
		RecordWidget->Stride = static_cast<float>(x) / Records.Num();
		RecordWidget->AddToViewport(999);
	}*/

	UUserWidget* GraphWidget = CreateWidget(GetWorld(), GraphWidgetClass);
	GraphWidget->AddToViewport();
}

void AMinerPlayerController::ProcessKneeTracking()
{
	PlayerMainAngle = ATransportManager::GetJointAngle();
	PlayerSubAngle1 = ATransportManager::GetSubAngle1();
	PlayerSubAngle2 = ATransportManager::GetSubAngle2();
	
	const float RaisedRate = PlayerMainAngle / PlayerLimitAngle;
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("rate: %f"), RaisedRate));
	
	// PlayerSubAngle1 허벅지와 종아리 안쪽 각도  70 <= x <= 100
	if(PlayerSubAngle1 < 70.0f || PlayerSubAngle1 > 100.0f)
	{
		if(WarningWidget->Visibility == ESlateVisibility::Hidden)
		{
			// incorrect webcam's depth tracking 
			WarningWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("무릎안쪽: %f"), PlayerSubAngle1));
		CntSubAngle1Failed++;
	}
	
	// PlayerSubAngle2 골반과 허벅지의 수평방향 각도  80 <= x <= 110
	if(PlayerSubAngle2 < 80.0f || PlayerSubAngle2 > 110.0f)
	{
		if(WarningWidget->Visibility == ESlateVisibility::Hidden)
		{
			WarningWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("골반: %f"), PlayerSubAngle2));
		CntSubAngle2Failed++;
	}

	if (CurrentStatus == EJointTrackingStatus::Standing)
	{
		PlayerPeakAngle = 0.0f;
		CntSubAngle1Failed = 0;
		CntSubAngle2Failed = 0;
		
		if(RaisedRate > LowerBoundRate)
		{
			CurrentStatus = EJointTrackingStatus::Rising;
			//CaptureComp->BeginCapture();
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
		PlayerPeakAngle = FMath::Max(PlayerPeakAngle, PlayerMainAngle);
		
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
			//CaptureComp->EndCapture();			
		}
	}
}
