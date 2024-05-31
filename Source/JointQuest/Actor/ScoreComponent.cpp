// JNU-JointQuest

#include "ScoreComponent.h"
#include "../GameInstance/JointQuestGameInstance.h"
#include "JointQuest/PlayerController/MinerPlayerController.h"
#include "Kismet/GameplayStatics.h"

UScoreComponent::UScoreComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UScoreComponent::RecordCurrentRep(bool bHasSucceeded)
{
	AMinerPlayerController* PC = Cast<AMinerPlayerController>(GetOwner());
	FExerciseRecord NewRecord(bHasSucceeded, PC->GetPeakAngle());

	GameInstance->RecordCurrentRep(NewRecord);

	if(GameInstance->GetRecords().Num() >= GoalReps)
	{
		PC->GameHasEnded(PC->GetOwner(), false);
	}
}

void UScoreComponent::AwardPoints(int32 Points)
{
	CurrentScore += Points;
}

float UScoreComponent::GetCurrentScore() const
{
	return CurrentScore;
}

void UScoreComponent::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UJointQuestGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(GameInstance);
}





