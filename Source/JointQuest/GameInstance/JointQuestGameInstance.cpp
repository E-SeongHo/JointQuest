// JNU-JointQuest


#include "JointQuestGameInstance.h"

void UJointQuestGameInstance::RecordCurrentRep(FExerciseRecord NewRecord)
{
	ExerciseRecords.Add(NewRecord);
	UE_LOG(LogTemp, Display, TEXT("%f"), NewRecord.PeakAngle);
}

const TArray<FExerciseRecord>& UJointQuestGameInstance::GetRecords() const
{
	return ExerciseRecords;
}

float UJointQuestGameInstance::GetRecordAngleRate(const int32 idx) const
{
	if(idx >= ExerciseRecords.Num()) return -1.0f;

	float Rate =  FMath::Clamp(ExerciseRecords[idx].PeakAngle / PlayerLimitAngle, 0.0f, 1.0f);
	return (Rate-0.8f) * 5;
}

bool UJointQuestGameInstance::HasRecordSucceeded(const int32 idx) const
{
	if(idx >= ExerciseRecords.Num()) return false;

	return ExerciseRecords[idx].bHasSucceeded;
}

float UJointQuestGameInstance::GetGameScore()
{
	return Score;
}

float UJointQuestGameInstance::GetGameMaxAngle()
{
	return MaxAngle;
}

int32 UJointQuestGameInstance::GetBestRepIndex()
{
	return BestRepIndex;
}

void UJointQuestGameInstance::SetCurrentRepAsBestRep()
{
	BestRepIndex = ExerciseRecords.Num()-1;
}

void UJointQuestGameInstance::GameEnd(const float GameScore)
{
	Score = GameScore;

	MaxAngle = 0.0f;
	for(auto R : ExerciseRecords)
	{
		MaxAngle = FMath::Max(MaxAngle, R.PeakAngle);
	}
}
