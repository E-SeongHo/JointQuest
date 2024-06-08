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
#include "Kismet/GameplayStatics.h"

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
	if(bHasGameEnded) return;
	
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

UCaptureComponent* AMinerPlayerController::GetCaptureComponent() const
{
	return CaptureComp;
}

void AMinerPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	//Super::GameHasEnded(EndGameFocus, bIsWinner);
	
	UJointQuestGameInstance* GameInstance = Cast<UJointQuestGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->GameEnd(ScoreComp->GetCurrentScore());
	
	for(int i = 0; i < 5; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Angle : %f, Succeeded : %d "),GameInstance->GetRecordAngleRate(i), GameInstance->HasRecordSucceeded(i));
	}

	CaptureComp->GameEnd();
	bHasGameEnded = true;

	// Make sure waits until the last rep is recorded properly
	FTimerHandle EndGameTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(EndGameTimerHandle, [this]()
	{
		UUserWidget* GraphWidget = CreateWidget(GetWorld(), GraphWidgetClass);
		GraphWidget->AddToViewport();
	}, 3.0f, false);
}

void AMinerPlayerController::ProcessKneeTracking()
{
	PlayerMainAngle = ATransportManager::GetJointAngle();
	PlayerSubAngle1 = ATransportManager::GetSubAngle1();
	PlayerSubAngle2 = ATransportManager::GetSubAngle2();

	// hack
	if(PlayerMainAngle > 0.0f)
	{
		bHasGameStarted = true;	
	}
	if(!bHasGameStarted) return;
	
	const float RaisedRate = PlayerMainAngle / PlayerLimitAngle;
	
	// PlayerSubAngle1 : Inner angle between thigh and calf 70 <= x <= 100
	if(PlayerSubAngle1 < 70.0f || PlayerSubAngle1 > 100.0f)
	{
		if(WarningWidget->Visibility == ESlateVisibility::Hidden)
		{
			// incorrect webcam's depth tracking 
			//WarningWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		CntSubAngle1Failed++;
	}
	
	// PlayerSubAngle2 Horizontal angle between pelvis and thigh 80 <= x <= 110
	if(PlayerSubAngle2 < 80.0f || PlayerSubAngle2 > 110.0f)
	{
		if(WarningWidget->GetVisibility() == ESlateVisibility::Hidden)
		{
			WarningWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		CntSubAngle2Failed++;
	}
	
	PlayerPeakAngle = FMath::Max(PlayerPeakAngle, PlayerMainAngle);
	
	if (CurrentStatus == EJointTrackingStatus::Standing)
	{
		PlayerPeakAngle = 0.0f;
		CntSubAngle1Failed = 0;
		CntSubAngle2Failed = 0;
		
		if(RaisedRate > LowerBoundRate)
		{
			CurrentStatus = EJointTrackingStatus::Rising;
			CaptureComp->BeginCapture();
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
			CaptureComp->EndCapture();			
		}
	}
}
