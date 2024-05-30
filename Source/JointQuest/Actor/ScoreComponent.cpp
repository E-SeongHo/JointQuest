// JNU-JointQuest

#include "ScoreComponent.h"

#include "JointQuest/PlayerController/MinerPlayerController.h"

UScoreComponent::UScoreComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UScoreComponent::RecordCurrentRep(bool bHasSucceeded)
{
	AMinerPlayerController* PC = Cast<AMinerPlayerController>(GetOwner());
	FExerciseRecord NewRecord(bHasSucceeded, PC->GetPeakAngle());

	ExerciseRecords.Add(NewRecord);

	if(ExerciseRecords.Num() >= GoalReps) PC->GameHasEnded(nullptr, false);
}

void UScoreComponent::AwardPoints(int32 Points)
{
	CurrentScore += Points;
}

float UScoreComponent::GetCurrentScore() const
{
	return CurrentScore;
}

TArray<FExerciseRecord> UScoreComponent::GetAllRecords() const
{
	return ExerciseRecords;
}





