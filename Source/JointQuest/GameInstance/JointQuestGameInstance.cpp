// JNU-JointQuest


#include "JointQuestGameInstance.h"

void UJointQuestGameInstance::RecordCurrentRep(FExerciseRecord NewRecord)
{
	ExerciseRecords.Add(NewRecord);
}

const TArray<FExerciseRecord>& UJointQuestGameInstance::GetRecords() const
{
	return ExerciseRecords;
}

float UJointQuestGameInstance::GetRecordAngleRate(const int32 idx) const
{
	if(idx >= ExerciseRecords.Num()) return -1.0f;

	return ExerciseRecords[idx].PeakAngle / PlayerLimitAngle;
}

bool UJointQuestGameInstance::HasRecordSucceeded(const int32 idx) const
{
	if(idx >= ExerciseRecords.Num()) return false;

	return ExerciseRecords[idx].bHasSucceeded;
}
